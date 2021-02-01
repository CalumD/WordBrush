import {logger} from '../logger';
import {NextFunction, Request, Response} from 'express';
import * as fs from 'fs-extra'
import {createHash} from 'crypto';

import {BASE_RESOURCES_PATH} from './wordbrush_interface'

const MINS_OLD_FOR_DELETION: number = 2;

type CacheEntry = {
    hash: string,
    url: string,
    outputDirectory: string
    fileName?: string,
}

class RequestCacheV1 {

    private hashToCache: Map<string, CacheEntry> = new Map<string, CacheEntry>();
    private dirToHash: Map<string, string> = new Map<string, string>();

    public constructor() {
        // Load existing past runs.
        this.populateCache();

        // Run an initial cleanup
        this.garbageCollect();

        // Kick off scheduled process to delete old entries.
        setInterval(() => {
            this.garbageCollect();
        }, MINS_OLD_FOR_DELETION * 1000 * 60)
    }

    private populateCache(): void {
        logger.notice("Performing first boot population of cache for existing outputs.");
        for (let dir of fs.readdirSync(`${BASE_RESOURCES_PATH}`)) {
            if (!fs.existsSync(`${BASE_RESOURCES_PATH}/${dir}/cache.json`)) {
                logger.inform(`Couldn't find cache file in output directory ${dir}, will purge.`);
                fs.rmdirSync(`${BASE_RESOURCES_PATH}/${dir}`, {recursive: true});
                continue;
            }
            this.put(fs.readJSONSync(`${BASE_RESOURCES_PATH}/${dir}/cache.json`));
        }
    }

    private garbageCollect(): void {
        logger.notice("Running garbage collector on output directories.");
        let folderAgeMins;

        for (const dir of Array.from(this.dirToHash.keys())) {
            folderAgeMins = RequestCacheV1.getFolderAgeInMinutes(`${BASE_RESOURCES_PATH}/${dir}`)
            logger.debug("Existing result set", {folder: dir, ageInMinutes: folderAgeMins})
            if (folderAgeMins > MINS_OLD_FOR_DELETION) {
                logger.inform(`Deleting result set > ${MINS_OLD_FOR_DELETION} mins old.`, {dir: dir})
                fs.rmdirSync(`${BASE_RESOURCES_PATH}/${dir}`, {recursive: true});
                this.hashToCache.delete(this.dirToHash.get(dir));
                this.dirToHash.delete(dir);
                logger.data(`${this.size()} values in the cache`);
            }
        }
        logger.success("Garbage collector complete.");
    }

    private static getFolderAgeInMinutes(path: string): number {
        return (Date.now() - new Date(fs.statSync(path).birthtime).getTime()) / 1000 / 60;
    }

    createCacheKey(url: string, file?: any): string {
        const hash = createHash('sha256').update(url);
        if (file) {
            hash.update(file.buffer);
        }
        return hash.digest('base64');
    }

    getDirFromHash(cacheKey: string): CacheEntry {
        return this.hashToCache.get(cacheKey);
    }

    put(value: CacheEntry): void {
        logger.info('Putting new value into cache', value)
        this.dirToHash.set(value.outputDirectory, value.hash);
        this.hashToCache.set(value.hash, value);
        logger.data(`${this.size()} values in the cache`)
    }

    size(): number {
        return this.hashToCache.size;
    }
}

const cache = new RequestCacheV1();


const tryCache = (req: Request, res: Response, next: NextFunction): string => {

    const hashResult = cache.createCacheKey(req.originalUrl, req.file);
    const cacheHIT = cache.getDirFromHash(hashResult);

    logger.debug("Cache " + (cacheHIT ? `HIT ${JSON.stringify(cacheHIT)}` : "MISS"))

    if (cacheHIT) {
        res.locals.data = {resultSetID: cacheHIT.outputDirectory};
        return 'HIT';
    }
    return hashResult;
}

const addToCache = (value: CacheEntry): void => {
    fs.writeFile(`${BASE_RESOURCES_PATH}/${value.outputDirectory}/cache.json`, JSON.stringify(value))
        .then(() => {
            logger.success("Successfully written new cache file for " + value.outputDirectory);
        })
        .catch((err) => {
            logger.error("Failed to write cache file for " + value.outputDirectory, err);
        });
    cache.put(value);
}

export {
    tryCache,
    addToCache
};

import {logger} from '../logger';
import {NextFunction, Request, Response} from 'express';
import * as fs from 'fs-extra'
import {createHash} from 'crypto';
import {resolve} from "path";
import {ErrorResponseBlock, RequestError} from "../errors";
import {ExecOutput} from "./wordbrush_interface";

const MINS_OLD_FOR_DELETION: number = 60;
export const BASE_RESOURCES_PATH: string = resolve(process.cwd() + '../../../resources');

type CacheEntry = {
    hash: string,
    url: string,
    outputDirectory: string
    fileName?: string,
    error?: ErrorResponseBlock
}


const deleteFolder = (dirToPurge: string): void => {
    logger.inform(`Deleting result set ${dirToPurge}.`);
    fs.rmSync(`${BASE_RESOURCES_PATH}/${dirToPurge}`, {recursive: true, force: true});
}

const tryCache = (req: Request, res: Response): string => {

    const hashResult = cache.createCacheKey(req.originalUrl, req.file);
    const cacheHIT = cache.getDirFromHash(hashResult);

    logger.data("Cache " + (cacheHIT ? `HIT ${JSON.stringify(cacheHIT)}` : "MISS"))

    if (cacheHIT) {
        res.locals.data = {resultSetID: cacheHIT.outputDirectory};
        return 'HIT';
    }
    return hashResult;
}

const addToCache = (value: CacheEntry): void => {
    fs.writeFileSync(`${BASE_RESOURCES_PATH}/${value.outputDirectory}/cache.json`, JSON.stringify(value));
    cache.put(value);
}

/**
 * If this search for a cache directory finds something, then it used to exist, and thus must be an error uuid.
 * If nothing was found, then there was no error and the user just searched for a dir which never existed.
 *
 * @param dirToCheck The uuid of the directory to check for
 * @param next This is used to respond with an error if required.
 */
const shouldErrorDirRequest = (dirToCheck: string, next: NextFunction): boolean => {
    let cacheErr: CacheEntry | ErrorResponseBlock = cache.getCacheFromDir(dirToCheck);
    if (cacheErr) {
        cacheErr = <ErrorResponseBlock>cacheErr.error;
        logger.inform("Request made for directory of failed process " + dirToCheck);
        next(new RequestError(cacheErr))
        return true;
    }
    return false;
}

const markExceptionDirectory = (dirToMark: string, execOutput: ExecOutput): void => {
    deleteFolder(dirToMark);
    let previousCache = cache.getCacheFromDir(dirToMark);
    previousCache.error = {
        code: 500,
        name: 'process_failed',
        description: 'The provided input caused an unexpected problem with the CLI.',
        message: 'CLI terminated prematurely.',
        data: execOutput
    }
    cache.put(previousCache);
    logger.inform(`Marked directory ${dirToMark} as a failed process.`)
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
                fs.rmSync(`${BASE_RESOURCES_PATH}/${dir}`, {recursive: true, force: true});
                continue;
            }
            this.put(fs.readJSONSync(`${BASE_RESOURCES_PATH}/${dir}/cache.json`));
        }
        logger.success("Cache primed.");
    }

    private garbageCollect(): void {
        logger.notice("Running garbage collector on output directories.");
        let folderAgeMins;

        for (const dir of Array.from(this.dirToHash.keys())) {
            if (this.getCacheFromDir(dir).error) {
                logger.trace(`Skipping GC for directory of failed process ${dir}`);
                continue;
            }
            folderAgeMins = RequestCacheV1.getFolderAgeInMinutes(`${BASE_RESOURCES_PATH}/${dir}`)
            logger.debug("Existing result set", {folder: dir, ageInMinutes: folderAgeMins})
            if (folderAgeMins > MINS_OLD_FOR_DELETION) {
                logger.inform(`Found result set > ${MINS_OLD_FOR_DELETION} mins old.`)
                deleteFolder(dir);
                this.hashToCache.delete(<string>this.dirToHash.get(dir));
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
        return <CacheEntry>this.hashToCache.get(cacheKey);
    }

    getCacheFromDir(dirOfBadRequest: string): CacheEntry {
        return <CacheEntry>this.hashToCache.get(<string>this.dirToHash.get(dirOfBadRequest));
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

export {
    tryCache,
    addToCache,
    shouldErrorDirRequest,
    markExceptionDirectory
};

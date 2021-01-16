import * as shell from 'shelljs';
import {logger} from "../logger";
import {resolve} from 'path';
import {NextFunction} from 'express';
import * as fs from 'fs-extra'
import {RequestError} from "../errors";
import {uuid} from "../uuid";

export const BASE_RESOURCES_PATH: string = resolve(process.cwd() + '../../../resources');
const APPLICATION_PATH: string = `${resolve(process.cwd() + '../../../compute/bin/wordbrush')}`;

export type ExecOutput = {
    code: number,
    stdOut: string,
    stdErr: string
}
export type WordBrushArgs = {
    width?: number,
    height?: number,
    sfo?: number,
    words?: string[],
    hasInputFile?: boolean
}

export type WordBrushMetadataFile = {
    outputType: "single" | "multi",
    words: {
        [key: string]: string | string[][]
    },
    startTime?: string,
    stopTime?: string,
    finalised?: boolean
}

const createResultSetDirectory = (): { path: string, id: string } => {
    const newResultSet = uuid.generate();
    const outputDirectory = `${BASE_RESOURCES_PATH}/${newResultSet}`;
    fs.ensureDirSync(outputDirectory);
    logger.note(`Created new result set directory: ${outputDirectory}`);

    return {path: outputDirectory, id: newResultSet};
}

export async function getWords(
    {wbArgs, file, next}: { wbArgs: WordBrushArgs; file: any, next: NextFunction }
): Promise<object> {
    return new Promise((resolve, reject) => {
        if (wbArgs.hasInputFile) {
            if (!file || !file.buffer) {
                return next(new RequestError({
                    code: 400,
                    name: 'bad_file_upload',
                    description: 'The uploaded file was rejected or corrupted, check filesize/extension is < 10MiB and .txt',
                    message: 'unacceptable file upload.'
                }));
            }
        } else {
            if (!wbArgs.words) {
                return next(new RequestError({
                    code: 400,
                    name: 'incomplete_request',
                    description: 'The request was missing either CLI input words, or a .txt file to use as input.',
                    message: 'No usable input present.'
                }));
            }
        }

        const outputData = createResultSetDirectory();
        let command: string;

        if (wbArgs.hasInputFile) {
            fs.writeFileSync(`${outputData.path}/input.txt`, file.buffer);
            command = `${APPLICATION_PATH} -i "${outputData.path}/input.txt"`;
        } else {
            command = APPLICATION_PATH;
        }

        const options =
            ` -o "${wbArgs.sfo ? outputData.path + '/0.svg' : outputData.path}"` +
            `${wbArgs.width ? ` -W ${wbArgs.width}` : ''}` +
            `${wbArgs.height ? ` -H ${wbArgs.height}` : ''}` +
            `${wbArgs.sfo ? ` -s ${wbArgs.sfo}` : ''}` +
            `${wbArgs.words ? ` ${wbArgs.words}` : ''}`;

        logger.debug("Calling WordBrush C code", {command: command, args: options});
        execAsync(`${command} ${options}`, {silent: true});

        resolve({resultSetID: outputData.id});
    });
}

export async function getOutput(
    {file, directory, next}: { file: string, directory: string, next: NextFunction }
): Promise<object> {
    return new Promise((resolve, reject) => {
        if (fs.existsSync(`${BASE_RESOURCES_PATH}/${directory}`)) {
            const fileName = `${BASE_RESOURCES_PATH}/${directory}/${file}`;
            if (fs.existsSync(fileName)) {
                resolve(fs.readFile(fileName, 'utf8'));
            } else {
                output404(file, directory, next);
            }
        } else {
            resultSet404(directory, next);
        }
    })
}

export async function getResultSet(
    {directory, next}: { directory: string, next: NextFunction }
): Promise<WordBrushMetadataFile> {
    return new Promise(async (resolve, reject) => {
        if (fs.existsSync(`${BASE_RESOURCES_PATH}/${directory}`)) {
            let metadataFilePath = (
                await execAsync(
                    `find ${BASE_RESOURCES_PATH}/${directory}/*.json`,
                    {silent: true}
                )
            ).stdOut;
            metadataFilePath = metadataFilePath.trim();
            try {
                const metadataFile: WordBrushMetadataFile = fs.readJSONSync(metadataFilePath);
                if (!metadataFile.finalised) {
                    throw new Error('Metadata file not finalised.');
                }
                resolve({outputType: metadataFile.outputType, words: metadataFile.words});
            } catch (Err) {
                console.error(Err);
                return resultSet202(directory, next);
            }
        } else {
            resultSet404(directory, next);
        }
        return;
    });
}

export async function getExistingResults(): Promise<any> {
    return new Promise(async (resolve, reject) => {
        return resolve(fs.readdir(BASE_RESOURCES_PATH));
    });
}

const resultSet404: Function = (directory: string, next: NextFunction): void => {
    next(new RequestError({
        code: 404,
        name: 'resultSet_not_found',
        description: 'The requested directory of output data was not found on the server.',
        data: directory
    }));
}
const resultSet202: Function = (directory: string, next: NextFunction): void => {
    next(new RequestError({
        code: 202,
        name: 'resultSet_not_ready',
        description: 'Requested directory of output data has not yet been finalised as the meta file wasn\'t found, or was marked as incomplete.',
        message: 'Data not ready yet, try again shortly.',
        data: directory
    }));
}
const output404: Function = (fileName: string, directory: string, next: NextFunction): void => {
    next(new RequestError({
        code: 404,
        name: 'output_file_not_found',
        description: `Requested file from existing result set could not be found.`,
        data: {resultSet: directory, file: fileName}
    }));
}

function execAsync(command: string, opts: {} = {}): Promise<ExecOutput> {
    return new Promise(function (resolve) {
        shell.exec(command, opts, function (code, stdout, stderr) {
            return resolve({
                code: code,
                stdOut: stdout == "" ? undefined : stdout,
                stdErr: stderr == "" ? undefined : stderr
            });
        });
    });
}
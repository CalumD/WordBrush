import * as shell from 'shelljs';
import {logger} from "../logger";
import {resolve, extname} from 'path';
import {NextFunction} from 'express';
import * as fs from 'fs-extra'
import {RequestError} from "../errors";
import {uuid} from "../uuid";
import {Middleware} from "../router/words_router_v1";

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
    words?: string[]
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

// export async function callApplication({width = 50, height = 50}: WordBrushArgs): Promise<ExecOutput> {
//     return execAsync('ls -lha');
// }

const createResultSetDirectory = (): { path: string, id: string } => {
    const newResultSet = uuid.generate();
    const outputDirectory = `${BASE_RESOURCES_PATH}/${newResultSet}`;
    fs.ensureDirSync(outputDirectory);

    return {path: outputDirectory, id: newResultSet};
}

export async function getWordsCLI(
    args: WordBrushArgs
): Promise<object> {
    return new Promise((resolve, reject) => {
        const outputData = createResultSetDirectory();

        if (args.sfo) {
            execAsync(`${APPLICATION_PATH} -o "${outputData.path}/0.svg" -W ${args.width} -H ${args.height} -s ${args.sfo} ${args.words}`, {silent: true});
        } else {
            execAsync(`${APPLICATION_PATH} -o "${outputData.path}" -W ${args.width} -H ${args.height} ${args.words}`, {silent: true});
        }

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
                resolve({outputType: metadataFile.outputType, words: metadataFile.words});
            } catch (Err) {
                return resultSet202(directory, next);
            }
        } else {
            resultSet404(directory, next);
        }
        return;
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
        description: 'The requested directory of output data has not yet been finalised as the meta file wasn\'t found.',
        message: 'Data not ready yet, try again shortly.',
        data: directory
    }));
}
const output404: Function = (fileName: string, directory: string, next: NextFunction): void => {
    next(new RequestError({
        code: 404,
        name: 'output_file_not_found',
        description: `The requested file from existing result set (${directory}) could not be found.`,
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
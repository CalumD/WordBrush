import * as shell from 'shelljs';
import {logger} from "../logger";
import {resolve, extname} from 'path';
import {NextFunction} from 'express';
import * as fs from 'fs-extra'
import {RequestError} from "../errors";

export const BASE_RESOURCES_PATH: string = resolve(process.cwd() + '../../../resources');

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

export async function callApplication({width = 50, height = 50}: WordBrushArgs): Promise<any> {
    return execAsync('ls -lha');
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

export async function getResultSet({directory, next}: { directory: string, next: NextFunction }): Promise<string[]> {
    return new Promise((resolve, reject) => {
        if (fs.existsSync(`${BASE_RESOURCES_PATH}/${directory}`)) {
            fs.readdir(`${BASE_RESOURCES_PATH}/${directory}`, (err, files) => {
                resolve(files.filter(f => extname(f).toLowerCase() === '.svg'));
            });
        } else {
            resultSet404(directory, next);
        }
    })
}

const resultSet404: Function = (directory: string, next: NextFunction): void => {
    next(new RequestError({
        code: 404,
        name: 'resultSet_not_found',
        description: 'The requested directory of output data was not found on the server.',
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
import * as shell from 'shelljs';
import {logger} from "../logger";
import {resolve} from "path";

export const BASE_RESOURCES_PATH: string = resolve(__dirname + '/../../resources');

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
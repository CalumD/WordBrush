import {logger} from './logger';
import {NextFunction, Request, Response, Router} from 'express';
import * as bodyParser from 'body-parser';
import {RequestError} from './errors';

export type Middleware =
    (req: Request, res: Response, next: NextFunction) => void;


const getWordsCLI: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called getWordsCLI');
    next();
}

const getWordsCLI_singleFileOutput: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called getWordsCLI_singleFileOutput')
    next();
}

const postWordsFile: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called postWordsFile')
    next();
}

const postWordsFile_singleFileOutput: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called postWordsFile_singleFileOutput')
    next();
}


export class WordsRouterV1 {

    public router: Router;

    public constructor() {
        this.router = Router();
        this.precondition();

        // CLI only params
        this.router.get('/words/w/:width/h/:height/sfo/:single_file_output_column_count', getWordsCLI_singleFileOutput);
        this.router.get('/words/w/:width/h/:height', getWordsCLI);

        // Input File in addition to CLI
        this.router.post('/words/w/:width/:height/sfo/:single_file_output_column_count', bodyParser.raw({limit: '1GB'}), postWordsFile_singleFileOutput)
        this.router.post('/words/w/:width/h/:height', bodyParser.raw({limit: '1GB'}), postWordsFile);
    }

    private static urlParamIntegerMatcher(input: string, forParam: string): void | RequestError {
        if (!input.match(/^[0-9]+$/)) {
            return new RequestError(400, 'invalid_size_param', 'URL parameter for ' + forParam + ' expected an integer, received non-integer input.');
        }
        return null;
    }

    private precondition(): void {
        this.router.param(
            'width',
            async (
                req: Request, res: Response, next: NextFunction,
                w: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamIntegerMatcher(w, 'width');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
        this.router.param(
            'height',
            async (
                req: Request, res: Response, next: NextFunction,
                h: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamIntegerMatcher(h, 'height');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
        this.router.param(
            'single_file_output_column_count',
            async (
                req: Request, res: Response, next: NextFunction,
                sfo: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamIntegerMatcher(sfo, 'sfo');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
    }
}

export default new WordsRouterV1().router;
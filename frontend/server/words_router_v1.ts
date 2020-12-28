import {logger} from './logger';
import {NextFunction, Request, Response, Router} from 'express';
import * as bodyParser from 'body-parser';
import {RequestError} from './errors';
import {uuid} from './uuid';

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

const getResultSet: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called getResultSet')
    next();
}

const getOutput: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called getOutput')
    next();
}


const validateQueryParamsRequired: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.trace('Called validateQueryParamsRequired')
    if (!req.query.input) {
        return next(new RequestError(400, 'missing_query_param', 'Required query param (input) was missing from the request.'));
    }
    next();
}

export class WordsRouterV1 {

    public router: Router;

    public constructor() {
        this.router = Router();
        this.precondition();

        // CLI only params
        this.router.get('/w/:width/h/:height/sfo/:single_file_output_column_count',
            validateQueryParamsRequired, getWordsCLI_singleFileOutput);
        this.router.get('/w/:width/h/:height',
            validateQueryParamsRequired, getWordsCLI);

        // Input File in addition to CLI
        this.router.post('/w/:width/:height/sfo/:single_file_output_column_count',
            bodyParser.raw({limit: '1GB'}), postWordsFile_singleFileOutput);
        this.router.post('/w/:width/h/:height',
            bodyParser.raw({limit: '1GB'}), postWordsFile);

        // A result set
        this.router.get('/results/:resultSet',
            getResultSet);
        this.router.get('/results/:resultSet/:output',
            getOutput);
    }

    private static urlParamIntegerMatcher(input: string, forParam: string): void | RequestError {
        if (!input.match(/^[0-9]+$/)) {
            return new RequestError(400, 'invalid_size_param', 'URL parameter for ' + forParam + ' expected an integer, received non-integer input.');
        }
        return null;
    }

    private static urlParamUUIDMatcher(input: string, forParam: string): void | RequestError {
        if (!uuid.verify(input)) {
            return new RequestError(400, 'invalid_uuid', 'URL parameter for ' + forParam + ' expected a valid uuid.');
        }
        return null;
    }

    private static urlParamFilenameMatcher(input: string, forParam: string): void | RequestError {
        if (input.match(/^([0-9]+)(\.svg)?$/)) {
            return new RequestError(400, 'invalid_filename', 'URL parameter for ' + forParam + ' expected /^([0-9]+)(\\.svg)?$/.');
        }
        return null;
    }

    private static integerBoundChecker(input: Number, forParam: string, minBound: Number, maxBound: Number): void | RequestError {
        if (input < minBound || input > maxBound) {
            return new RequestError(400, 'invalid_size_param', `URL parameter for ${forParam} expected to be (${minBound} <= x <= ${maxBound}).`);
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
                potentialError = WordsRouterV1.integerBoundChecker(+w, 'width', 50, 3840);
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
                potentialError = WordsRouterV1.integerBoundChecker(+h, 'height', 50, 3840);
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
                potentialError = WordsRouterV1.integerBoundChecker(+sfo, 'sfo', 1, 100);
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
        this.router.param(
            'resultSet',
            async (
                req: Request, res: Response, next: NextFunction,
                results: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamUUIDMatcher(results, 'resultSet');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
        this.router.param(
            'output',
            async (
                req: Request, res: Response, next: NextFunction,
                output: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamFilenameMatcher(output, 'output');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
    }
}

export default new WordsRouterV1().router;
import {logger} from './logger';
import {NextFunction, Request, Response, Router} from 'express';
import * as bodyParser from 'body-parser';
import {RequestError} from './errors';
import {uuid} from './uuid';
import {resolve} from 'path';

export type Middleware =
    (req: Request, res: Response, next: NextFunction) => void;


const getWordsCLI: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    logger.follow('Called getWordsCLI');
    res.locals.methodCalled = 'Called getWordsCLI';
    next();
}

const getWordsCLI_singleFileOutput: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    logger.follow('Called getWordsCLI_singleFileOutput');
    res.locals.methodCalled = 'Called getWordsCLI_singleFileOutput';
    next();
}

const postWordsFile: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    logger.follow('Called postWordsFile');
    res.locals.methodCalled = 'Called postWordsFile';
    next();
}

const postWordsFile_singleFileOutput: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    logger.follow('Called postWordsFile_singleFileOutput');
    res.locals.methodCalled = 'Called postWordsFile_singleFileOutput';
    next();
}

const getResultSet: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    logger.follow('Called getResultSet');
    res.locals.methodCalled = 'Called getResultSet';
    next();
}

const getOutput: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    logger.follow('Called getOutput');
    res.locals.methodCalled = 'Called getOutput';
    next();
}

const getMethods: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.follow('Called getMethods');
    res.locals.methodCalled = 'Called getMethods';
    // TODO: Improve this by making it programmatic!
    res.locals.data = {
        endpoints: [
            'api/v1/words/w/:width/h/:height/sfo/:single_file_output_column_count',
            'api/v1/words/w/:width/h/:height',
            'api/v1/words/w/:width/:height/sfo/:single_file_output_column_count',
            'api/v1/words/w/:width/h/:height',
            'api/v1/words/results/:resultSet',
            'api/v1/words/results/:resultSet/:output'
        ]
    };
    next();
}

const validateQueryParamsRequired: Middleware = (
    req: Request, res: Response, next: NextFunction
): void => {
    logger.trace('Called validateQueryParamsRequired')
    if (!req.query.input) {
        return next(new RequestError({
            code: 400,
            name: 'missing_query_param',
            description: 'Required query param (input) was missing from the request.'
        }));
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

        this.router.get('', getMethods);
    }

    private static urlParamIntegerMatcher(input: string, forParam: string): void | RequestError {
        if (!input.match(/^[0-9]+$/)) {
            return new RequestError({
                code: 400,
                name: 'invalid_size_param',
                description: 'URL parameter for ' + forParam + ' expected an integer, received non-integer input.'
            });
        }
        return null;
    }

    private static urlParamUUIDMatcher(input: string, forParam: string): void | RequestError {
        if (!uuid.verify(input)) {
            return new RequestError({
                code: 400,
                name: 'invalid_uuid',
                description: 'URL parameter for ' + forParam + ' expected a valid uuid.'
            });
        }
        return null;
    }

    private static urlParamFilenameMatcher(input: string, forParam: string): void | RequestError {
        if (input.match(/^([0-9]+)(\.svg)?$/)) {
            return new RequestError({
                code: 400,
                name: 'invalid_filename',
                description: 'URL parameter for ' + forParam + ' expected /^([0-9]+)(\\.svg)?$/.'
            });
        }
        return null;
    }

    private static integerBoundChecker(input: Number, forParam: string, minBound: Number, maxBound: Number): void | RequestError {
        if (input < minBound || input > maxBound) {
            return new RequestError({
                code: 400,
                name: 'invalid_size_param',
                description: 'URL parameter for ${forParam} expected to be (${minBound} <= x <= ${maxBound}).'
            });
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
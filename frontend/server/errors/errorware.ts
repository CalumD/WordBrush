import {NextFunction, Request, Response} from 'express';
import {logger} from '../logger';

export interface ErrorResponseBlock {
    status: Number;
    error: string;
    error_description?: string;
    user_message?: string;
    error_data?: any;
}

export class RequestError extends Error {

    public static is(test: object): test is RequestError {
        return (test as Partial<RequestError>).code !== undefined
            && (test as Partial<RequestError>).name !== undefined
            && (test as Partial<RequestError>).responseBlock !== undefined;
    }

    public code: number;
    public userMessage?: string;
    public errorData?: any;

    public constructor(
        errCode: number = 500,
        errName: string = 'internal_server_error',
        errDescription?: string,
        errUserMessage?: string,
        errData?: any
    ) {
        super(errDescription);
        Object.setPrototypeOf(this, new.target.prototype);
        this.code = errCode;
        this.name = errName;
        this.userMessage = errUserMessage;
        this.errorData = errData;
    }

    public responseBlock(): ErrorResponseBlock {
        return {
            status: this.code,
            error: this.name,
            error_description: this.message,
            user_message: this.userMessage,
            error_data: this.errorData
        };
    }
}

export type ErrorWare = (err: Error, req: Request, res: Response, next: NextFunction) => void;

export const errorHandler: ErrorWare = (err: Error, req: Request, res: Response, next: NextFunction): void => {
    if (RequestError.is(err)) {
        res.status(err.code).json(err.responseBlock());
        logger.error(`Responded to ${req.ip} with error in ${res.locals.responseTime}`, err.responseBlock());
    } else {
        res.status(500).send(err.stack);
        logger.error(`Responded to ${req.ip} with error in ${res.locals.responseTime}`, err);
    }
};

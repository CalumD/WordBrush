import {NextFunction, Request, Response} from 'express';
import {logger} from '../logger';

export interface ErrorResponseBlock {
    code: number;
    name: string;
    description?: string;
    message?: string;
    data?: any;
}

export class RequestError extends Error {

    public code: number;
    public userMessage?: string;
    public errorData?: any;

    public constructor({code = 500, name = 'internal_server_error', description, message, data}: ErrorResponseBlock) {
        super(description);
        Object.setPrototypeOf(this, new.target.prototype);
        this.code = code;
        this.name = name;
        this.userMessage = message;
        this.errorData = data;
    }

    public static is(test: object): test is RequestError {
        return (test as Partial<RequestError>).code !== undefined
            && (test as Partial<RequestError>).name !== undefined
            && (test as Partial<RequestError>).responseBlock !== undefined;
    }

    public responseBlock(): ErrorResponseBlock {
        return {
            code: this.code,
            name: this.name,
            description: this.message,
            message: this.userMessage,
            data: this.errorData
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

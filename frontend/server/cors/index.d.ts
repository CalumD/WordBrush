import { NextFunction, Request, Response } from 'express';

export interface CORSOptions {
	origins?: string[] | Function;
	methods?: string[];
	requestHeaders?: string[];
	responseHeaders?: string[];
	supportsCredentials?: boolean;
	maxAge?: number;
	endPreflightRequests?: boolean;
}

export const cors: () =>
	(req: Request, res: Response, next: NextFunction) =>
		void;

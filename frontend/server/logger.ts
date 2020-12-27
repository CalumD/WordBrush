import * as winston from 'winston';


interface WinstonCustomLogLevels {
    levels: {
        [key: string]: number
    },
    colors: {
        [key: string]: string
    }
}

interface LogLevel {
    rank: number,
    colour: string,
    names: string[]
}

const LOG_LEVELS: LogLevel[] = [
    {
        rank: 7,
        colour: 'cyan',
        names: ['silly', 'testing', 'verbose']
    },
    {
        rank: 6,
        colour: 'magenta',
        names: ['debug', 'minor', 'data']
    },
    {
        rank: 5,
        colour: 'white',
        names: ['information', 'inform', 'info']
    },
    {
        rank: 4,
        colour: 'green',
        names: ['note', 'notice', 'notify', 'success', 'prompt', 'help']
    },
    {
        rank: 3,
        colour: 'yellow',
        names: ['warn', 'warning']
    },
    {
        rank: 2,
        colour: 'yellow',
        names: ['err', 'error', 'fail', 'failure']
    },
    {
        rank: 1,
        colour: 'red',
        names: ['severe', 'alert', 'breach']
    },
    {
        rank: 0,
        colour: 'red',
        names: ['critical', 'fatal', 'emergency', 'shutdown']
    }
];

const customLogLevelsForWinston: WinstonCustomLogLevels = {
    levels: {},
    colors: {}
};

LOG_LEVELS.forEach((level: LogLevel): void => {
    level.names.forEach((name: string): void => {
        customLogLevelsForWinston.levels[name] = level.rank;
        customLogLevelsForWinston.colors[name] = level.colour;
    })
})

export interface Logger {
    // 7
    silly: (message: string, meta?: {}) => void,
    testing: (message: string, meta?: {}) => void,
    verbose: (message: string, meta?: {}) => void,
    // 6
    debug: (message: string, meta?: {}) => void,
    minor: (message: string, meta?: {}) => void,
    data: (message: string, meta?: {}) => void,
    // 5
    information: (message: string, meta?: {}) => void,
    inform: (message: string, meta?: {}) => void,
    info: (message: string, meta?: {}) => void,
    // 4
    note: (message: string, meta?: {}) => void,
    notice: (message: string, meta?: {}) => void,
    notify: (message: string, meta?: {}) => void,
    success: (message: string, meta?: {}) => void,
    prompt: (message: string, meta?: {}) => void,
    help: (message: string, meta?: {}) => void,
    // 3
    warn: (message: string, meta?: {}) => void,
    warning: (message: string, meta?: {}) => void,
    // 2
    err: (message: string, meta?: {}) => void,
    error: (message: string, meta?: {}) => void,
    fail: (message: string, meta?: {}) => void,
    failure: (message: string, meta?: {}) => void,
    // 1
    severe: (message: string, meta?: {}) => void,
    alert: (message: string, meta?: {}) => void,
    breach: (message: string, meta?: {}) => void,
    // 0
    critical: (message: string, meta?: {}) => void,
    fatal: (message: string, meta?: {}) => void,
    emergency: (message: string, meta?: {}) => void,
    shutdown: (message: string, meta?: {}) => void
}

let basicConsoleLogger;

const setDefaults: (defaults: { component: string; }) => Logger =
    (defaults): Logger => {

        basicConsoleLogger = winston.createLogger({
            levels: customLogLevelsForWinston.levels,
            transports: [new winston.transports.Console()],
            exitOnError: false,
            handleExceptions: true,
            level: 'verbose',
            format: winston.format.combine(
                winston.format.label({label: defaults.component}),
                winston.format.timestamp({format: 'YYYY-MM-DD HH:mm:ss'}),
                winston.format.align(),
                winston.format.printf(({level, message, timestamp, label, ...data}) => {
                    return winston.format.colorize()
                        .colorize(level,
                            `[${label}] [${timestamp}] [${level.toUpperCase()}] ${message}`
                        ) + `${Object.keys(data).length > 0 ? `\n\t\t\t\t\t\t${JSON.stringify(data)}` : ''}`
                })
            )
        });
        winston.addColors(customLogLevelsForWinston.colors);

        return {
            // 7
            silly: (message, meta) => basicConsoleLogger['silly'](message, meta),
            testing: (message, meta) => basicConsoleLogger['testing'](message, meta),
            verbose: (message, meta) => basicConsoleLogger['verbose'](message, meta),
            // 6
            debug: (message, meta) => basicConsoleLogger['debug'](message, meta),
            minor: (message, meta) => basicConsoleLogger['minor'](message, meta),
            data: (message, meta) => basicConsoleLogger['data'](message, meta),
            // 5
            information: (message, meta) => basicConsoleLogger['information'](message, meta),
            inform: (message, meta) => basicConsoleLogger['inform'](message, meta),
            info: (message, meta) => basicConsoleLogger['info'](message, meta),
            // 4
            note: (message, meta) => basicConsoleLogger['note'](message, meta),
            notice: (message, meta) => basicConsoleLogger['notice'](message, meta),
            notify: (message, meta) => basicConsoleLogger['notify'](message, meta),
            prompt: (message, meta) => basicConsoleLogger['prompt'](message, meta),
            success: (message, meta) => basicConsoleLogger['success'](message, meta),
            help: (message, meta) => basicConsoleLogger['help'](message, meta),
            // 3
            warn: (message, meta) => basicConsoleLogger['warn'](message, meta),
            warning: (message, meta) => basicConsoleLogger['warning'](message, meta),
            // 2
            err: (message, meta) => basicConsoleLogger['err'](message, meta),
            error: (message, meta) => basicConsoleLogger['error'](message, meta),
            fail: (message, meta) => basicConsoleLogger['fail'](message, meta),
            failure: (message, meta) => basicConsoleLogger['failure'](message, meta),
            // 1
            severe: (message, meta) => basicConsoleLogger['severe'](message, meta),
            alert: (message, meta) => basicConsoleLogger['alert'](message, meta),
            breach: (message, meta) => basicConsoleLogger['breach'](message, meta),
            // 0
            critical: (message, meta) => basicConsoleLogger['critical'](message, meta),
            fatal: (message, meta) => basicConsoleLogger['fatal'](message, meta),
            emergency: (message, meta) => basicConsoleLogger['emergency'](message, meta),
            shutdown: (message, meta) => basicConsoleLogger['shutdown'](message, meta),
        }
    }

export const logger: Logger = setDefaults({component: 'WordBrush'});


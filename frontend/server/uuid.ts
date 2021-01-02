import {v4, validate} from 'uuid';

export const uuid: {
    generate: () => string;
    verify: (input: string) => boolean;
} = {generate: v4, verify: validate};

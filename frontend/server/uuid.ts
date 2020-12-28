import {v5, validate} from 'uuid';

export const uuid: {
    generate: () => string;
    verify: (input: string) => boolean;
} = {generate: v5, verify: validate};

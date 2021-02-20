import {ref} from 'vue';

const drawWithColour = ref(false);

export function useColourPaths() {

    return {
        drawWithColour
    };
}
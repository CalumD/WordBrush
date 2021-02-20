import {ref} from 'vue';
import {nanoid} from "nanoid";

export function useUniqueId(length = 5) {

    const nextId = ref(nanoid(length));

    function uniqueIdValue() {
        // Ensure the id does not start with a number.
        do {
            nextId.value = nanoid(length);
        } while (nextId.value.match(/^\d.*/));

        return nextId;
    }

    return {
        uniqueIdValue
    };
}
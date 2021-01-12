import {ref} from 'vue';
import {nanoid} from "nanoid";

export function useUniqueId(length = 5) {

    const nextId = ref(nanoid(length));

    function uniqueIdValue() {
        return nextId.value;
    }

    return {
        uniqueIdValue
    };
}
import {ref} from 'vue';

const showAllWordText = ref(false);

export function useShowAllWordText() {

    function toggleShowAllWordText() {
        showAllWordText.value = !showAllWordText.value;
    }

    return {
        showAllWordText,
        toggleShowAllWordText
    };
}
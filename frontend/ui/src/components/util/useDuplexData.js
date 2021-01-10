import {computed} from 'vue'

export function useDuplexData(
    props,
    emit,
    {
        name = 'modelValue',
        outputMutator = (val) => {
            return val;
        }
    }
) {
    return computed({
        get: () => props[name],
        set: (value) => emit(`update:${name}`, outputMutator(value))
    })
}

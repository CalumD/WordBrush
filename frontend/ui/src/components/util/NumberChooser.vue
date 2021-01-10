<template>
    <label class="number_chooser">
        <input
            type="number"
            :min="min"
            :max="max"
            v-model="result"
        />
    </label>
</template>

<script>
import {useDuplexData} from "@/components/util/useDuplexData";

export default {
    name: "NumberChooser",
    props: {
        min: {
            type: Number,
            default: 0
        },
        max: {
            type: Number,
            default: 10
        },
        result: Number
    },
    setup(props, {emit}) {
        return {
            result: useDuplexData(props, emit, {name: 'result', outputMutator: (num) => parseInt(num, 10)}),
        }
    }
}
</script>

<style scoped>
.number_chooser {
    width: 0;
}

.number_chooser input {
    width: 4rem;
}

</style>
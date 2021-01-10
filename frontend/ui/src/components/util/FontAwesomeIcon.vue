<template>
    <svg xmlns="http://www.w3.org/2000/svg" :class="$props.class" :viewBox="`0 0 ${width} ${height}`">

        <path fill="currentColor" :d="svgPath"/>

    </svg>
</template>

<script>
import {computed} from "vue";
import {faHistory, faSearch, faTimes} from '@fortawesome/free-solid-svg-icons'
import {findIconDefinition, library} from "@fortawesome/fontawesome-svg-core";

library.add(faSearch, faHistory, faTimes)

export default {
    name: "FontAwesomeIcon",
    props: {
        icon: {
            type: String,
            required: true
        },
        from_library: {
            type: String,
            default: "fas",
            required: false
        },
        class: String
    },

    setup(props) {
        const definition = computed(() =>
            findIconDefinition({
                prefix: props.from_library,
                iconName: props.icon
            })
        );

        const width = computed(() => definition.value.icon[0]);
        const height = computed(() => definition.value.icon[1]);
        const svgPath = computed(() => definition.value.icon[4]);

        return {width, height, svgPath};
    }
}
</script>
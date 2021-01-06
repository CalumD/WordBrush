<template>
    <div class="row content">
        <HelloWorld msg="Welcome to Your Vue.js App"/>
    </div>
</template>

<script>
import HelloWorld from "@/components/HelloWorld";

export default {
    name: 'Display',
    components: {HelloWorld},
    props: [],
    data() {
        return {}
    },
    methods: {
        method_1: function (arg_1) {
        }
    },
    computed: {}
}

</script>

<style scoped>

</style>
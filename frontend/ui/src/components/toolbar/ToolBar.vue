<template>
    <div id="toolbar">
        <LogoStamp/>
        <Search/>
        <PreviousOutput :existing_outputs="existing_outputs"/>
    </div>
</template>


<script>
import axios from 'axios';
import LogoStamp from "@/components/toolbar/LogoStamp";
import Search from "@/components/toolbar/Search";
import PreviousOutput from "./PreviousOutput";

export default {
    name: 'ToolBar',
    components: {
        LogoStamp, Search, PreviousOutput
    },
    data() {
        return {
            existing_outputs: []
        }
    },
    methods: {
        fetchExistingOutput: function () {
            axios
                .get('http://ubuntu.vms.local:3000/api/v1/results')
                .then((out) => {
                    this.existing_outputs = out.data
                })
                .catch((err) => {
                    console.log("Failed to retrieve existing results data", {err: err});
                    this.existing_outputs = [];
                });
        }
    },
    mounted() {
        this.fetchExistingOutput();
    }
}

</script>


<style scoped>
#toolbar {
    height: 100%;
    background: red;
    grid-area: toolbar;
    display: flex;
    flex-flow: row wrap;
    overflow: auto;
    align-items: center;
    align-self: center;
}

#toolbar > * {
    display: flex;
    padding: 5px;
    align-self: inherit;
    height: 100%;
}
</style>
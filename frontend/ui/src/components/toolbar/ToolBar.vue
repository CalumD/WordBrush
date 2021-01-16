<template>
    <div id="toolbar">
        <LogoStamp/>
        <Search/>
        <PreviousOutput/>
    </div>
</template>


<script>
import LogoStamp from "@/components/toolbar/LogoStamp";
import Search from "@/components/toolbar/Search";
import PreviousOutput from "./PreviousOutput";

export default {
    name: 'ToolBar',
    components: {
        LogoStamp, Search, PreviousOutput
    }
}

</script>


<style scoped>
#toolbar {
    height: 100%;
    background-image: linear-gradient(to top, #aaa5bd, #2f2f2f);
    grid-area: toolbar;
    display: flex;
    flex-flow: row wrap;
    overflow: auto;
    align-items: center;
    align-self: center;
    /*box-shadow: 0 4px 2px -2px gray;*/
}

#toolbar > * {
    display: flex;
    padding: 5px;
    align-self: inherit;
    height: 100%;
}
</style>
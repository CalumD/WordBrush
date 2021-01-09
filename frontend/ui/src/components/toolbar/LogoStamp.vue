<template>
    <div
        id="logo_container"
        @mouseenter="toggleShowAllWordText"
        @mouseleave="toggleShowAllWordText"
    >
        <img id="wb_logo" alt="WordBrush written in WordBrush as logo." src="../../assets/WordBrush.svg">
        <h1>WordBrush</h1>
    </div>
</template>


<script>
import {useShowAllWordText} from "@/components/util/showAllWordText";

export default {
    name: "LogoStamp",
    setup() {
        return useShowAllWordText();
    }
}
</script>


<style scoped>
#logo_container {
    display: flex;
    justify-content: flex-start;
    flex: 1 4;
}

#logo_container h1 {
    align-self: center;
}

#wb_logo {
    max-height: 100%;
}
</style>
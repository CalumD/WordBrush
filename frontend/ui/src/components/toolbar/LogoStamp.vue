<template>
    <div
        id="logo_container"
        @mouseenter="toggleShowAllWordText"
        @mouseleave="toggleShowAllWordText"
    >
        <img id="wb_logo" alt="WordBrush written in WordBrush as logo." src="../../assets/WordBrush.svg">
        <h1><strong>W</strong>ord<strong>B</strong>rush</h1>
    </div>
</template>


<script>
import {useShowAllWordText} from "@/components/util/useShowAllWordText";

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
    min-width: 430px;
}

#logo_container h1 {
    align-self: center;
    margin-left: -70px;
    font-size: 2.5rem;
    margin-bottom: .5rem;
    font-weight: 500;
    line-height: 1.2;
    margin-top: 0;
}

#logo_container strong {
    font-weight: bolder;
}

#wb_logo {
    max-height: 100%;
}
</style>
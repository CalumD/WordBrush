<template>
    <div id="previous_container">
        <div v-if="existing_outputs.length > 0">
            <p> {{ existing_outputs }} </p>
        </div>
        <div v-else>
            <p>No existing data found.</p>
        </div>
    </div>
</template>

<script>
export default {
    name: "PreviousOutput",
    props: {
        existing_outputs: Array
    }
}
</script>

<style scoped>

#previous_container {
    display: flex;
    justify-content: flex-end;
    flex: 0.5 4;
    max-height: 100%;
}
</style>
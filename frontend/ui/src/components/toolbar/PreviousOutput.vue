<template>
    <div id="previous_container">
        <div v-if="existing_outputs.length > 0">
            <div v-for="past in existing_outputs">
                <p> {{ past }} </p>
            </div>
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
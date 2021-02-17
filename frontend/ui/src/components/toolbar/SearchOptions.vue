<template>
    <div id="search_options_container" v-click-away="close">
        <button type="button" class="options_button" @click="viewingOptions = !viewingOptions">
            <transition name="options_button_transition" mode="out-in">
                <FontAwesomeIcon v-if="!viewingOptions" class="options_icon" icon="cogs"></FontAwesomeIcon>
                <FontAwesomeIcon v-else class="options_icon" icon="times"></FontAwesomeIcon>
            </transition>
        </button>
        <transition name="dropdown">
            <div v-if="viewingOptions" class="options_dropdown_wrapper">
                <ul class="options_dropdown_content">
                    <li>
                        <span>Width</span>
                        <NumberChooser :min="50" :max="3840" v-model:result="opts.width"></NumberChooser>
                    </li>
                    <li>
                        <span>Height</span>
                        <NumberChooser :min="50" :max="3840" v-model:result="opts.height"></NumberChooser>
                    </li>
                    <li>
                        <span>No. Columns</span>
                        <NumberChooser :min="1" :max="100" v-model:result="opts.sfo"></NumberChooser>
                    </li>
                    <li>
                        <span>Draw Keyboard</span>
                        <label class="ticky_box">
                            <input type="checkbox" v-model="opts.drawKeyboard"/>
                        </label>
                    </li>
                    <li>
                        <span>Enable Colour Tracing (Experimental)</span>
                        <label class="ticky_box">
                            <input type="checkbox" v-model="opts.traceColour"/>
                        </label>
                    </li>
                </ul>
            </div>
        </transition>
    </div>
</template>

<script>
import FontAwesomeIcon from '@/components/util/FontAwesomeIcon'
import NumberChooser from '@/components/util/NumberChooser'
import {useDuplexData} from "@/components/util/useDuplexData";

export default {
    name: "SearchOptions",
    components: {NumberChooser, FontAwesomeIcon},
    props: {
        opts: {
            type: Object
        }
    },
    data() {
        return {
            viewingOptions: false
        }
    },
    setup(props, {emit}) {
        return {
            result: useDuplexData(props, emit, {name: 'opts'})
        }
    },
    methods: {
        close: function () {
            this.viewingOptions = false;
        }
    }
}
</script>

<style scoped>

#search_options_container {
    display: flex;
    align-items: center;
    justify-content: center;
    flex: 2 2;
    max-height: 100%;
    max-width: 110px;
}

.options_button {
    height: 60%;
    width: 60%;
    outline: none;
    background-color: #e3e3e3;
    border-color: black;
    border-radius: 10px;
}

.options_icon {
    height: 75%;
    width: 75%;
}

.options_button_transition-enter-active,
.options_button_transition-leave-active {
    transition: all 0.3s ease-out;
}

.options_button_transition-enter-from,
.options_button_transition-leave-to {
    opacity: 0;
}

.dropdown-enter-active,
.dropdown-leave-active {
    transition: all 0.6s;
}

.dropdown-enter-from,
.dropdown-leave-to {
    opacity: 0;
    transform: translateY(30px);
}


.options_dropdown_wrapper {
    display: flow-root;
    position: absolute;
    z-index: 999;
    top: 120px;
    right: 100px;
    height: 50%;
    min-height: 200px;
    max-height: 80%;
    min-width: 350px;
    width: 35%;
    max-width: 50%;
    border-radius: 12px;
    /*padding: 12px 0 12px 0;*/
    background-color: white;
    border: 2px solid gray;
    overflow-y: hidden;
}

.options_dropdown_content {
    display: flex;
    flex-flow: column nowrap;
    overflow-y: scroll;
    height: 100%;
    max-width: 100%;
    width: auto;
}

.options_dropdown_content li {
    display: flex;
    flex-flow: row nowrap;
    flex-shrink: 1;
    align-items: center;
    max-height: 40px;
    text-decoration: none;
    padding: 5px 5% 5px 5%;
}


.options_dropdown_wrapper li:nth-child(even) {
    background: #efefef;
}

.options_dropdown_wrapper li:nth-child(odd) {
    background: #dbdbdb;
}

.options_dropdown_wrapper li:hover {
    color: #17bf63;
    background-color: rgba(79, 192, 141, 0.3);
    border-radius: 5px;
}

.options_dropdown_wrapper span {
    flex-shrink: 1;
    text-align: left;
    padding-right: 30px;
    width: 60%;
}

.options_dropdown_wrapper .number_chooser,
.options_dropdown_wrapper .ticky_box {
    width: 40%;
}

.ticky_box {
    display: flex;
    justify-content: center;
    width: 100%;
}

</style>
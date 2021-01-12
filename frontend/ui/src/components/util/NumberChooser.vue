<template>
    <label class="number_chooser">
        <input
            class="num_chooser_input"
            type="number"
            pattern="[0-9]*"
            :min="min"
            :max="max"
            :step="step"
            v-model="result"
        />
        <button
            type="button"
            @mousedown="startDecrement"
            @mouseleave="stopChanging"
            @mouseup="stopChanging"
            class="num_chooser_button_minus">
            <FontAwesomeIcon icon="minus"/>
        </button>
        <button
            type="button"
            @mousedown="startIncrement"
            @mouseleave="stopChanging"
            @mouseup="stopChanging"
            class="num_chooser_button_plus">
            <FontAwesomeIcon icon="plus"/>
        </button>
    </label>
</template>

<script>
import {useDuplexData} from "@/components/util/useDuplexData";
import FontAwesomeIcon from "@/components/util/FontAwesomeIcon";

const clickHoldChangeInitialSpeed = 500;
const clickHoldChangeRate = 1.5;
const clickHoldChangeMaxSpeed = 35;

export default {
    name: "NumberChooser",
    components: {FontAwesomeIcon},
    props: {
        min: {
            type: Number,
            default: 0
        },
        step: {
            type: Number,
            default: 1
        },
        max: {
            type: Number,
            default: 10
        },
        result: Number
    },
    data() {
        return {
            isDecrementing: false,
            isIncrementing: false,
            currentChangeRate: 500
        }
    },
    setup(props, {emit}) {
        return {
            result: useDuplexData(props, emit, {name: 'result', outputMutator: (num) => parseInt(num, 10)})
        }
    },
    methods: {
        updateChangeRate() {
            return ((this.currentChangeRate / clickHoldChangeRate) < clickHoldChangeMaxSpeed)
                ? clickHoldChangeMaxSpeed
                : (this.currentChangeRate / clickHoldChangeRate);
        },
        startDecrement: function () {
            if ((this.result - this.step) >= this.min) {
                if (this.result > this.max) {
                    this.result = this.max;
                } else {
                    this.result -= this.step;
                }
                this.isDecrementing = setTimeout(this.startDecrement, this.currentChangeRate);
                this.currentChangeRate = this.updateChangeRate();
            } else {
                this.result = 0;
                this.stopChanging();
            }
        },
        startIncrement: function () {
            if ((this.result + this.step) <= this.max) {
                if (this.result < this.min) {
                    this.result = this.min;
                } else {
                    this.result += this.step;
                }
                this.isIncrementing = setTimeout(this.startIncrement, this.currentChangeRate);
                this.currentChangeRate = this.updateChangeRate();
            } else {
                this.result = 9999;
                this.stopChanging();
            }
        },
        stopChanging: function () {
            if (this.isDecrementing) {
                clearTimeout(this.isDecrementing);
            }
            if (this.isIncrementing) {
                clearTimeout(this.isIncrementing);
            }
            this.currentChangeRate = clickHoldChangeInitialSpeed;
        }
    }
};

</script>

<style scoped>

.number_chooser {
    display: flex;
    flex-flow: row nowrap;
    height: 30px;
    outline: none;
}

.number_chooser button {
    height: 100%;
    width: 100%;
    flex-grow: 1;
    flex-shrink: 0;
    flex-basis: 40%;
    background-color: white;
    outline: none;
}

button svg {
    height: 60%;
    width: 60%;
}

.num_chooser_button_minus {
    border-top-left-radius: 15px;
    border-bottom-left-radius: 15px;
    order: 0;
}

.num_chooser_input {
    -moz-appearance: textfield;
    text-align: center;
    height: 100%;
    width: 100%;
    flex-grow: 2;
    flex-shrink: 0;
    outline: none;
    order: 1;
}

.num_chooser_button_plus {
    border-top-right-radius: 15px;
    border-bottom-right-radius: 15px;
    order: 2;
}


.num_chooser_input:invalid {
    box-shadow: none;
    background-color: #e3e3e3;
    color: #6d6d6d;
}

.num_chooser_input::-webkit-inner-spin-button,
.num_chooser_input::-webkit-outer-spin-button,
.num_chooser_input:hover::-webkit-inner-spin-button,
.num_chooser_input:hover::-webkit-outer-spin-button {
    -moz-appearance: none;
    -webkit-appearance: none;
    appearance: none;
    margin: 0;
}
</style>
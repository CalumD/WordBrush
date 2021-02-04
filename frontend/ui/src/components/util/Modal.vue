<template>
    <Teleport to="#modals">
        <transition name="modal-transition" mode="out-in" appear>
            <div v-if="show" v-show="!exiting" class="modal-wrapper">
                <transition name="bounce" appear>
                    <div v-if="!exiting" class="modal-container" v-click-away="close">
                        <div class="modal-content">
                            <h1>{{ heading }}</h1>
                            <p style="white-space: pre-line">{{ body }}</p>
                        </div>
                        <div class="modal-buttons">
                            <button @click="okay" type="button">
                                {{ okayText }}
                            </button>
                            <button v-if="showCancel" @click="cancel" type="button">
                                {{ cancelText }}
                            </button>
                        </div>
                    </div>
                </transition>
            </div>
        </transition>
    </Teleport>
</template>

<script>
import {useDuplexData} from "@/components/util/useDuplexData";

export default {
    name: "Modal",
    props: {
        show: {
            type: Boolean,
            default: false
        },
        heading: {
            type: String,
            default: 'Modal Heading'
        },
        body: {
            type: String,
            default: 'Modal Body Text'
        },
        okayText: {
            type: String,
            default: 'Okay'
        },
        showCancel: {
            type: Boolean,
            default: false
        },
        cancelText: {
            type: String,
            default: 'Cancel'
        },
        accepted: {
            type: Boolean,
            default: true
        }
    },
    data() {
        return {
            ignoreFirstClickAway: true,
            exiting: false
        }
    },
    methods: {
        close: function () {
            if (this.ignoreFirstClickAway) {
                this.ignoreFirstClickAway = false;
                return;
            }
            this.accepted = false;
            this.animateOut();
        },
        okay: function () {
            this.accepted = true;
            this.animateOut();
        },
        cancel: function () {
            this.accepted = false;
            this.animateOut();
        },
        animateOut: function () {
            this.exiting = true;
            setTimeout(() => {
                this.show = false
            }, 500)
        }
    },
    setup(props, {emit}) {
        return {
            show: useDuplexData(props, emit, {name: 'show'}),
            accepted: useDuplexData(props, emit, {name: 'accepted'})
        }
    }
}
</script>

<style scoped>
.modal-wrapper {
    float: left;
    z-index: 999;
    background-color: rgba(0, 0, 0, 0.7);
    min-width: 100%;
    min-height: 100%;
    position: absolute;
    left: 0;
    top: 0;
    transition: all .5s ease;
}

.modal-container {
    background-color: #b6b6b6;
    color: white;
    min-width: 36%;
    max-width: 36%;
    min-height: 36%;
    max-height: 36%;
    position: absolute;
    left: calc((100% / 2) - 18%);
    top: calc((100% / 2) - 30%);
    display: flex;
    flex-flow: column nowrap;
    border-radius: 10px;
}

.modal-buttons {
    display: flex;
    flex-flow: row nowrap;
    min-width: 100%;
    justify-content: space-around;
    padding-bottom: 10px;
}

.modal-content {
    flex-grow: 2;
    min-height: 50px;
    min-width: 100%;
    display: flex;
    flex-flow: column nowrap;
    justify-content: center;
    text-align: center;
}

.modal-content h1 {
    color: black;
    flex-grow: 1;
    font-size: 32px;
    text-decoration: underline;
}

.modal-content p {
    flex-grow: 2;
    overflow-y: auto;
    font-size: 20px;
}

.modal-buttons button {
    min-height: 50px;
    min-width: 25%;
    max-width: 50%;
    border-color: rgba(0, 0, 0, 64);
    border-radius: 20px;
    border-width: 2px;
    border-style: solid;
    color: red;
}

.modal-buttons button:hover {
    background-color: #b8b8b8;
}

.modal-container * {
    font-size: large;
    font-weight: bold;
    opacity: 100%;
}

.modal-transition-enter-from,
.modal-transition-leave-to {
    opacity: 0;
}

.bounce-enter-active {
    animation: bounce-in 0.5s;
}

.bounce-leave-active {
    animation: bounce-out 0.5s;
}

@keyframes bounce-in {
    0% {
        transform: scale(0);
    }
    50% {
        transform: scale(1.15);
    }
    100% {
        transform: scale(1);
    }
}

@keyframes bounce-out {
    0% {
        transform: scale(1);
    }
    25% {
        transform: scale(1.25);
    }
    100% {
        transform: scale(0);
    }
}

</style>
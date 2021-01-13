export default {
    beforeMount: function (el, binding) {
        binding.event = function (event) {
            if (!(el === event.target || el.contains(event.target))) {
                if (binding.value instanceof Function) {
                    binding.value(event)
                }
            }
        }
        document.body.addEventListener('click', binding.event)
        document.body.addEventListener('touchend', binding.event);
    },
    unmounted: function (el, binding) {
        document.body.removeEventListener('click', binding.event)
        document.body.removeEventListener('touchend', binding.event)
    }
}
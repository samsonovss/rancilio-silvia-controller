from esphome import pins
import esphome.codegen as cg
from esphome.components import output
import esphome.config_validation as cv
from esphome.const import CONF_ID

gpio_ns = cg.esphome_ns.namespace("gpio")

ac_cycle_skip_ns = cg.esphome_ns.namespace("ac_cycle_skip")
ACCycleSkipOutput = ac_cycle_skip_ns.class_(
    "ACCycleSkipOutput", output.FloatOutput, cg.Component
)

ZC_INTERRUPT_TYPES = {
    "RISING": gpio_ns.INTERRUPT_RISING_EDGE,
    "FALLING": gpio_ns.INTERRUPT_FALLING_EDGE,
    "ANY": gpio_ns.INTERRUPT_ANY_EDGE,
}

CONF_GATE_PIN = "gate_pin"
CONF_ZERO_CROSS_PIN = "zero_cross_pin"
CONF_ZERO_CROSS_INTERRUPT_TYPE = "zero_cross_interrupt_type"
CONF_NOISE_FILTER_US = "noise_filter_us"
CONF_MIN_ZERO_CROSS_INTERVAL_US = "min_zero_cross_interval_us"
CONF_MAX_ZERO_CROSS_INTERVAL_US = "max_zero_cross_interval_us"
CONF_GATE_PULSE_US = "gate_pulse_us"
CONF_START_BOOST_MS = "start_boost_ms"
CONF_RAMP_MS = "ramp_ms"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(ACCycleSkipOutput),
        cv.Required(CONF_GATE_PIN): pins.internal_gpio_output_pin_schema,
        cv.Required(CONF_ZERO_CROSS_PIN): pins.internal_gpio_input_pin_schema,
        cv.Optional(CONF_ZERO_CROSS_INTERRUPT_TYPE, default="FALLING"): cv.enum(
            ZC_INTERRUPT_TYPES, upper=True, space="_"
        ),
        cv.Optional(CONF_NOISE_FILTER_US, default=5000): cv.int_range(
            min=1000, max=9000
        ),
        cv.Optional(CONF_MIN_ZERO_CROSS_INTERVAL_US, default=6000): cv.int_range(
            min=3000, max=12000
        ),
        cv.Optional(CONF_MAX_ZERO_CROSS_INTERVAL_US, default=13000): cv.int_range(
            min=9000, max=30000
        ),
        cv.Optional(CONF_GATE_PULSE_US, default=200): cv.int_range(
            min=50, max=1000
        ),
        cv.Optional(CONF_START_BOOST_MS, default=0): cv.int_range(
            min=0, max=1000
        ),
        cv.Optional(CONF_RAMP_MS, default=800): cv.int_range(min=0, max=5000),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_GATE_PIN])
    cg.add(var.set_gate_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_ZERO_CROSS_PIN])
    cg.add(var.set_zero_cross_pin(pin))
    cg.add(var.set_zero_cross_interrupt_type(config[CONF_ZERO_CROSS_INTERRUPT_TYPE]))
    cg.add(var.set_noise_filter_us(config[CONF_NOISE_FILTER_US]))
    cg.add(var.set_min_zero_cross_interval_us(config[CONF_MIN_ZERO_CROSS_INTERVAL_US]))
    cg.add(var.set_max_zero_cross_interval_us(config[CONF_MAX_ZERO_CROSS_INTERVAL_US]))
    cg.add(var.set_gate_pulse_us(config[CONF_GATE_PULSE_US]))
    cg.add(var.set_start_boost_ms(config[CONF_START_BOOST_MS]))
    cg.add(var.set_ramp_ms(config[CONF_RAMP_MS]))

import esphome.codegen as cg
from esphome.components import i2c, sensor, text_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_PRESSURE,
    CONF_TEMPERATURE,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_TEMPERATURE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PASCAL,
)

DEPENDENCIES = ["i2c"]

CONF_PRESSURE_RANGE_BAR = "pressure_range_bar"
CONF_START_ERRORS = "start_errors"
CONF_STATUS_ERRORS = "status_errors"
CONF_MEASUREMENT_TIMEOUTS = "measurement_timeouts"
CONF_PACKET_ERRORS = "packet_errors"
CONF_REJECTED_PACKETS = "rejected_packets"
CONF_TOTAL_ERRORS = "total_errors"
CONF_CONSECUTIVE_ERRORS = "consecutive_errors"
CONF_LAST_ERROR = "last_error"
CONF_LINK_ERRORS = "link_errors"
CONF_PRESSURE_REJECTED = "pressure_rejected"
CONF_TEMPERATURE_REJECTED = "temperature_rejected"
CONF_LAST_REJECTION = "last_rejection"
CONF_HEALTH = "health"

ERROR_COUNTER_SCHEMA = sensor.sensor_schema(
    accuracy_decimals=0,
    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
)

xdb401_ns = cg.esphome_ns.namespace("xdb401")
XDB401Component = xdb401_ns.class_(
    "XDB401Component", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(XDB401Component),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PASCAL,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PRESSURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PRESSURE_RANGE_BAR, default=12): cv.one_of(
                1, 2, 5, 10, 12, 20, 50, 100, int=True
            ),
            cv.Optional(CONF_START_ERRORS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_STATUS_ERRORS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_MEASUREMENT_TIMEOUTS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_PACKET_ERRORS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_REJECTED_PACKETS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_TOTAL_ERRORS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_CONSECUTIVE_ERRORS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_LAST_ERROR): text_sensor.text_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_LINK_ERRORS): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_PRESSURE_REJECTED): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_TEMPERATURE_REJECTED): ERROR_COUNTER_SCHEMA,
            cv.Optional(CONF_LAST_REJECTION): text_sensor.text_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_HEALTH): text_sensor.text_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x7F))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_pressure_range_bar(config[CONF_PRESSURE_RANGE_BAR]))

    if temperature_config := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature_config)
        cg.add(var.set_temperature_sensor(sens))
    if pressure_config := config.get(CONF_PRESSURE):
        sens = await sensor.new_sensor(pressure_config)
        cg.add(var.set_pressure_sensor(sens))

    diagnostic_sensors = {
        CONF_START_ERRORS: var.set_start_error_sensor,
        CONF_STATUS_ERRORS: var.set_status_error_sensor,
        CONF_MEASUREMENT_TIMEOUTS: var.set_measurement_timeout_sensor,
        CONF_PACKET_ERRORS: var.set_packet_error_sensor,
        CONF_REJECTED_PACKETS: var.set_rejected_packet_sensor,
        CONF_TOTAL_ERRORS: var.set_total_error_sensor,
        CONF_CONSECUTIVE_ERRORS: var.set_consecutive_error_sensor,
        CONF_LINK_ERRORS: var.set_link_error_sensor,
        CONF_PRESSURE_REJECTED: var.set_pressure_rejected_sensor,
        CONF_TEMPERATURE_REJECTED: var.set_temperature_rejected_sensor,
    }
    for key, setter in diagnostic_sensors.items():
        if sensor_config := config.get(key):
            sens = await sensor.new_sensor(sensor_config)
            cg.add(setter(sens))

    text_sensors = {
        CONF_LAST_ERROR: var.set_last_error_sensor,
        CONF_LAST_REJECTION: var.set_last_rejection_sensor,
        CONF_HEALTH: var.set_health_sensor,
    }
    for key, setter in text_sensors.items():
        if text_config := config.get(key):
            sens = await text_sensor.new_text_sensor(text_config)
            cg.add(setter(sens))

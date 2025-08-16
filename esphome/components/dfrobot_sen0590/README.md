# dfrobot_sen0590

DFRobot SEN0590 Laser Ranging Sensor (4m) component for ESPHome.

## Example configuration:

```yaml
i2c:
  sda: GPIO32
  scl: GPIO33
  scan: true

sensor:
  - platform: dfrobot_sen0590
    name: "Laser Distance"
    address: 0x74  # Default address is 0x74
    update_interval: 1s
    filters:
      - median:
          window_size: 5
          send_every: 1
```

## Notes:
- Default I2C address is 0x74
- Sensor range: 20mm to 4000mm
- Accuracy depends on the surface being measured
- Using filters is recommended for stable readings

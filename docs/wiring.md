# Подключение / Wiring

## Назначение GPIO

| Функция | GPIO | Примечание |
|---|---:|---|
| Силовое реле машины | GPIO4 | Активный низкий уровень |
| Штатная кнопка Power | GPIO5 | Вход с внутренней подтяжкой |
| Статусный LED | GPIO6 | Цифровой выход |
| SSR нагревателя | GPIO7 | `slow_pwm`, период 1 с |
| SPI CLK | GPIO11 | MAX31865 |
| SPI MISO | GPIO12 | MAX31865 |
| SPI MOSI | GPIO13 | MAX31865 |
| MAX31865 CS | GPIO14 | Выбор устройства SPI |
| Датчик уровня воды | GPIO18 | XKC-Y25-NPN, open-collector, активный LOW |

## PT100 и MAX31865

Конфигурация рассчитана на трёхпроводной PT100, опорный резистор 430 Ом и сетевой фильтр 50 Гц. Проверьте перемычки и фактический опорный резистор своей платы MAX31865.

## Датчик воды XKC-Y25-NPN

GPIO ESP32-S3 рассчитаны на 3,3 В и не являются устойчивыми к 5 В. XKC-Y25-NPN использует open-collector, поэтому сигнальный выход подтягивается внутренним резистором ESP32 к 3,3 В.

- коричневый: питание датчика +5 В;
- синий: GND датчика и общий GND с ESP32;
- жёлтый: сигнальный выход на GPIO18;
- чёрный: выбор логики выхода, подключается согласно маркировке конкретной ревизии датчика.

Не устанавливайте внешнюю подтяжку жёлтого провода к 5 или 12 В. Конфигурация использует `pullup: true` и `inverted: true`, поэтому состояние `ON` означает наличие воды.

## Mains wiring

The mains and low-voltage sections must be physically separated. Use correctly rated relays, SSRs, terminals, wire, insulation, fusing, and protective earth. The exact mains wiring is intentionally not documented until the assembled machine has been verified.

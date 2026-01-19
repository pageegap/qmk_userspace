TRI_LAYER_ENABLE = yes
TAP_DANCE_ENABLE = yes
COMBO_ENABLE = yes
SEND_STRING_ENABLE = yes
# testing encoder function
ifneq ($(strip $(MCU)), atmega32u4)
ifneq ($(strip $(CONVERT_TO)), stemcell)
ENCODER_ENABLE = yes
ENCODER_MAP_ENABLE = yes
endif
endif
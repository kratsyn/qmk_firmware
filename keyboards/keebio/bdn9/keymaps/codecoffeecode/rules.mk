<<<<<<< HEAD
MOUSEKEY_ENABLE = yes
ifneq (, $(findstring rev1, $(KEYBOARD)))
  CTPC = yes
endif
=======
MOUSEKEY_ENABLE = yes 
LTO_ENABLE=yes      # compact this firmware a bit so it will pass avr size check
CTCP=yes
>>>>>>> a2b998919... modify codecoffeecode keymap to allow compiling for avr

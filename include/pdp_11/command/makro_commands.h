#define MAKRO_COMMAND_PROTOTYPE(NAME_COMMANDS)                                \
    void command_do_##NAME_COMMANDS(                                          \
        struct pdp_11_t *, address_word_t, word_t, byte_t)

extern byte_t set_has_b;

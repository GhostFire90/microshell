#include "ush.h"
#include "ush_internal.h"
#include "ush_shell.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(desc != NULL);

        USH_ASSERT(desc->input_buffer != NULL);
        USH_ASSERT(desc->input_buffer_size > 0);
        USH_ASSERT(desc->output_buffer != NULL);
        USH_ASSERT(desc->output_buffer_size > 0);

        USH_ASSERT(desc->io != NULL);
        USH_ASSERT(desc->io->read != NULL);
        USH_ASSERT(desc->io->write != NULL);

        USH_ASSERT(desc->hostname != NULL);

        self->desc = desc;
        self->root = NULL;

        ush_reset(self);
}

bool ush_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool busy = false;

        if (ush_reset_service(self) != false)
                return true;        
        if (ush_prompt_service(self) != false)
                return true;        
        if (ush_read_service(self, &busy) != false)
                return busy;        
        if (ush_parse_service(self) != false)
                return true;        
        if (ush_write_service(self) != false)
                return true;
        
        USH_ASSERT(false);
        
        return false;
}

void ush_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        ush_reset_start(self);
}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        char *ret = (char*)ush_utils_get_status_string(status);
        ush_write_pointer(self, ret, USH_STATE_RESET);
}

void ush_print(struct ush_object *self, char *buf)
{
        ush_write_pointer(self, buf, USH_STATE_RESET);
}

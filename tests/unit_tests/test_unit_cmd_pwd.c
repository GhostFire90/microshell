#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "inc/ush_commands.h"

int g_assert_call_count;

struct ush_object ush;

ush_status_t ush_print_status_status;
int ush_print_status_call_count;

char* ush_print_buf;
int ush_print_call_count;

extern void ush_buildin_cmd_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        ush_print_buf = NULL;
        ush_print_call_count = 0;
}

void tearDown(void)
{

}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_print_status_status, status);

        ush_print_status_call_count++;
}

void ush_print(struct ush_object *self, char *buf)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_print_buf, buf);

        ush_print_call_count++;
}

void test_ush_buildin_cmd_pwd_callback_neg(void)
{
        for (int i = 0; i < 6; i++) {
                setUp();
                if (i != 1) {
                        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                        ush_buildin_cmd_pwd_callback(&ush, NULL, i, NULL);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_call_count);
                }
        }
}

void test_ush_buildin_cmd_pwd_callback_pos(void)
{
        struct ush_node_object node;
        node.path = "test";

        ush.current_node = &node;
        char *argv[2] = {0};
        
        ush_print_buf = "test";
        ush_buildin_cmd_pwd_callback(&ush, NULL, 1, argv);
        TEST_ASSERT_EQUAL(1, ush_print_call_count);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_pwd_callback_neg);
        RUN_TEST(test_ush_buildin_cmd_pwd_callback_pos);

        return UNITY_END();
}

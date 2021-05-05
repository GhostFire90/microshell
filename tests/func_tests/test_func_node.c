#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <microshell.h>
#include "test_func.h"

static struct ush_node_object node = {0};

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{
        test_func_deinit();
}

void test_node(void)
{
        TEST_ASSERT_NULL(ush_node_get_by_path(&g_ush, "/test"));
        TEST_ASSERT_NULL(ush_node_get_by_path(&g_ush, "/data/binary"));
        TEST_ASSERT_NULL(ush_node_get_by_path(&g_ush, "/dir/../"));

        TEST_ASSERT_EQUAL(&g_path_root, ush_node_get_by_path(&g_ush, "/"));
        TEST_ASSERT_EQUAL(&g_path_data, ush_node_get_by_path(&g_ush, "/data"));
        TEST_ASSERT_EQUAL(&g_path_dir, ush_node_get_by_path(&g_ush, "/dir"));
        TEST_ASSERT_EQUAL(&g_path_dir111, ush_node_get_by_path(&g_ush, "/dir/1/11/111"));

        TEST_ASSERT_NULL(ush_node_get_by_path(&g_ush, "/abc"));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_mount(&g_ush, "/abc", &node, NULL, 0));
        TEST_ASSERT_EQUAL(&node, ush_node_get_by_path(&g_ush, "/abc"));
}

void test_node_parent(void)
{
        TEST_ASSERT_EQUAL(&g_path_root, ush_node_get_parent_by_path(&g_ush, "/test"));
        TEST_ASSERT_EQUAL(&g_path_data, ush_node_get_parent_by_path(&g_ush, "/data/binary"));
        TEST_ASSERT_NULL(ush_node_get_parent_by_path(&g_ush, "/dir/../"));

        TEST_ASSERT_NULL(ush_node_get_parent_by_path(&g_ush, "/"));
        TEST_ASSERT_EQUAL(&g_path_root, ush_node_get_parent_by_path(&g_ush, "/data"));
        TEST_ASSERT_EQUAL(&g_path_root, ush_node_get_parent_by_path(&g_ush, "/dir"));
        TEST_ASSERT_EQUAL(&g_path_dir11, ush_node_get_parent_by_path(&g_ush, "/dir/1/11/111"));

        TEST_ASSERT_EQUAL(&g_path_root, ush_node_get_parent_by_path(&g_ush, "/abc"));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_mount(&g_ush, "/abc", &node, NULL, 0));
        TEST_ASSERT_EQUAL(&g_path_root, ush_node_get_parent_by_path(&g_ush, "/abc"));
}

#define TEST_NODE_ABS_PATH(expected, input) \
{\
        ush_node_get_absolute_path(&g_ush, input, buf);\
        TEST_ASSERT_EQUAL_STRING(expected, buf);\
}

void test_node_abs(void)
{
        char buf[TEST_FUNC_WORK_BUFFER_SIZE];

        TEST_NODE_ABS_PATH("/", "/");
        TEST_NODE_ABS_PATH("/test", "/test");
        TEST_NODE_ABS_PATH("/", "/test/..");
        TEST_NODE_ABS_PATH("/", "/test/../");
        TEST_NODE_ABS_PATH("/abc", "/test/../abc");
        TEST_NODE_ABS_PATH("/abc", "/test/../abc/");
        TEST_NODE_ABS_PATH("/abc", "/test/.././abc/");
        TEST_NODE_ABS_PATH("/qwerty", "/test/../abc/../../../qwerty");
        TEST_NODE_ABS_PATH("/qwerty/xyz", "/test/../abc/../../../qwerty/xyz");
}

void test_node_abs_deeper(void)
{
        char buf[TEST_FUNC_WORK_BUFFER_SIZE];

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/dir/1/11/111"));

        test_node_abs();

        TEST_NODE_ABS_PATH("/dir/1/11/111", "");
        TEST_NODE_ABS_PATH("/dir/1/11/111", ".");
        TEST_NODE_ABS_PATH("/dir/1/11/111/test", "./test");
        TEST_NODE_ABS_PATH("/dir/1/11/111/test", "test");
        TEST_NODE_ABS_PATH("/dir/1/11/111", "test/..");
        TEST_NODE_ABS_PATH("/dir/1/11/111", "test/../");
        TEST_NODE_ABS_PATH("/dir/1/11/111/abc", "test/../abc");
        TEST_NODE_ABS_PATH("/dir/1/11/111/abc", "test/../abc/");
        TEST_NODE_ABS_PATH("/dir/1/11/111/abc", "test/.././abc/");
        TEST_NODE_ABS_PATH("/dir/1/qwerty", "test/../abc/../../../qwerty");
        TEST_NODE_ABS_PATH("/dir/1/qwerty/xyz", "test/../abc/../../../qwerty/xyz");
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_node);
        RUN_TEST(test_node_parent);
        RUN_TEST(test_node_abs);
        RUN_TEST(test_node_abs_deeper);

        return UNITY_END();
}

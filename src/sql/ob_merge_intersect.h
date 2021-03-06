/**
 * (C) 2010-2012 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * ob_merge_union.h
 *
 * Authors:
 *   TIAN GUAN <tianguan.dgb@taobao.com>
 *
 */
#ifndef OCEANBASE_SQL_OB_MERGE_INTERSECT_H_
#define OCEANBASE_SQL_OB_MERGE_INTERSECT_H_

#include "sql/ob_set_operator.h"
#include "common/ob_row.h"

namespace oceanbase
{
  namespace sql
  {
    class ObMergeIntersect: public ObSetOperator
    {
      public:
        ObMergeIntersect();
        virtual ~ObMergeIntersect();
        virtual int open();
        virtual int close();
        /**
         * 获得下一行的引用
         * @note 在下次调用get_next或者close前，返回的row有效
         * @pre 调用open()
         * @param row [out]
         *
         * @return OB_SUCCESS或OB_ITER_END或错误码
         */
        virtual int get_next_row(const common::ObRow *&row);
        virtual int get_row_desc(const common::ObRowDesc *&row_desc) const;
        virtual int64_t to_string(char* buf, const int64_t buf_len) const;
        virtual int set_distinct(bool is_distinct);
      private:
        int cons_row_desc();
        DISALLOW_COPY_AND_ASSIGN(ObMergeIntersect);
      private:
        typedef int (ObMergeIntersect::*get_next_row_func_type)(const common::ObRow *&row);
        get_next_row_func_type get_next_row_func_;
        int distinct_get_next_row(const common::ObRow *&row);
        int all_get_next_row(const common::ObRow *&row);
        int compare(const common::ObRow *this_row, const common::ObRow *last_row, int &cmp) const;
        int do_distinct(ObPhyOperator *op, const common::ObRow *other, const common::ObRow *&row);
      private:
        common::ObRow left_last_row_;
        common::ObRow right_last_row_;
        const common::ObRow *cur_first_query_row_;
        const common::ObRow *cur_second_query_row_;
        int left_ret_;
        int right_ret_;
        int last_cmp_;
        bool got_first_row_;
        char *left_last_row_buf_;
        char *right_last_row_buf_;
        static const uint64_t OB_ROW_BUF_SIZE = common::OB_MAX_ROW_LENGTH;
    };
  } // end namespace sql
} // end namespace oceanbase

#endif /* OCEANBASE_SQL_OB_MERGE_INTERSECT_H_ */

/*
 * @Author: your name
 * @Date: 2021-12-07 00:36:14
 * @LastEditTime: 2021-12-13 00:01:03
 * @LastEditors: Karigen B
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\FANode.h
 */
#pragma once
struct FANode
{
    int input;
    int output;
    FANode() = default;
    FANode(int input_, int output_) {
        this->input = input_;
        this->output = output_;
    }
};


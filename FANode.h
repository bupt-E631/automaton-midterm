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


<!--
 * @Author: your name
 * @Date: 2021-11-10 11:25:05
 * @LastEditTime: 2021-12-05 13:41:06
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\README.md
-->

# Automaton-Midterm Work

> Contributor : JohnZhang , XiaoZhu , YuhangYang , ChengyuLiu
>
> Student number : 2020211809 , 2020211810 , 2020522059 , 2020522064
>
> Class : 2020211320

## 注意事项：

- 每次开始写代码之前一定要先从 main 分支拉取！！
- 每次拉取前需要先把自己的仓库贮藏，否则拉取会失败！！
- 而且每次写完代码推送之前一定要先清理解决方案！！
- 推送到自己的分支！！

## 2021/11/10 Edition.1

- 创建了版本库(第一次用 github 托管团队项目)
- 梦开始的地方...

## 转化步骤

$正则表达式\to \varepsilon-NFA\to NFA\to DFA\to 最小化DFA$

## 2021/12/6 Edition.2

- 一个月后终于来更新了...
- 一个自动机包含状态集合，转移函数，开始状态，结束状态集合；所有状态用数字命名

## 2021/12/7

- -1 代表空转移
- Node 里的 input 代表输入字符，output 代表转移状态

## 2021/12/8

- 卧槽!!!!!我$\varepsilon-NFA\to NFA$一遍过了!!!!!调试都没调试!!!!!
- 我太佩服我自己了，允许我自己夸一下自己-zx
- zx别摸鱼了！-zyh-hehe

##2021/12/12

- 夜深了。现在是12号的凌晨一点半。当然，这个点我从来就没有睡过觉。
- 打了好多天代码，终于完成了$RegularExpression \rightarrow \epsilon -NFA $的转化，挺高兴，但想到自己还有一堆作业和实验（比如数据结构）没写，一堆课没学（自动机：懂的都懂，这玩意就不是给人学的 ，哈工大王春宇老师yyds ; 大雾：当朝大学物理统共有十三章，朕不得不一章没学，你tm多给几天不行啊），又高兴不起来。
-  写写转化思路吧，反正也睡不着：
- 首先，由于输入的正则表达式中省略了乘号，所以我们必须将其预处理，把缺失的乘号补上（用&表示）
- 然后，要将预处理后的表达式又中缀形式转化成逆波兰式（后缀式），以使机器可读
- 接下来使用Thompson算法将该后缀表达式转化成一个epsilon-NFA
- Thompson算法的大致思想是，从一个小型的NFA开始，不断将其和另一个NFA串联/并联，或者自身形成闭包，最后递归地生成正确的NFA。具体算法如下：
- 我们对后缀表达式逐字符读取，如果遇到：
- 0/1，就把它转化成一个只有两个状态的epsilon-NFA 并压入栈中; 
- +号，取出栈顶两台epsilon-NFA并将其串联（有对应函数执行此过程）
- &号，取出栈顶两台epsilon-NFA并将其并联（有对应函数执行此过程）
- *号，取出栈顶的epsilon-NFA并将闭包化（有对应函数执行此过程）
- 读取完后缀表达式后，栈中只会剩下一台epsilon-NFA ,即为所求。
- 我写完了，yyh加油，别学你那破爪哇了，作业都写不完了，赶紧把NFA转DFA的代码给写完
- 以上 By zyh-hehe


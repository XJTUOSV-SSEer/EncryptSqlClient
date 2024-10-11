# ReadMe

本仓库用于存储关于可搜索加密数据库客户端方面的代码，目前尚未完成

## To-Do

- [x] 构建 pgSql connector 

- [x] DataMapper：可以根据传入的参数从数据构建对应的 MultiMap 和 EMM

- [x] 完善 Setup、Token函数

- [ ] DataMapper：根据数据类型进行对应的加密

- [ ] 完成对 Key 的持久化

- [ ] ...
## 完成情况简述

1. MM MultiMap，OPX 论文实现的一个多映射结构，对于$MM_R$ 而言，它的标准形式是 $\{Index,CiperValues\}$ ，Index 表示表-行索引，即指代当前行位于中哪一个表中的哪一列，CipherValues 表示加密后的行数据。

   1. 代码中的实现为 RowMultiMap,当前建立 mm 需要依靠 DataMapper 类，这个类持有 key 的相关信息，

   2. 索引格式暂时使用 x,y 的字符串形式

   3. 加密数据应当根据不同类型进行加密，仅查询的数据使用对称加密，仅计算的数据使用同态加密，不同加密的 Key 应当妥善保存。

2. EMM EncryptedMultiMap，经过加密的 MM 结构，真正在数据库存储的结构，目前仅实现了行等值查询的$EMM_R$

   1. 对于$EMM_R$ ，它的格式为$<Enc_{K}(Index),Enc_{K1}(Val)>$ ，前者为经过加密的索引信息，即当前值位于哪一个表的哪一行的哪一列，目前使用的 prf 函数进行加密，后者就是对应的加密值，与 MM 中直接对应。
3. DataMapper：具有有关数据载入与持久化、建立 MM 的功能，计划能够根据不同输入建立不同的 MM。
4. EncrtyptManager：执行论文中所实现的 Setup 和 Token 功能的类

      
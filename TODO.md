导出的时候也要统计时间，成功: 共创建 1827 个日报文件，已保存至: C:\Computer\my_github\github_cpp\time_tracer\my_test\output\exported_files\Typst_logs\days



高优先级

-1 周报告修复变量错误的使用导致不显示内容
# 2025-W01

**2024-12-30****2025-01-05**

- ****: 113h 11m (average: 16h 10m/day)
- ****: 5

-2 更新周报告的批量导出

-3 dll配置传递使用c接口
现在是传递toml

0
支持txt增量插入，存储已经存入sqlite的txt的哈希值，如果哈希值改变，则说明txt有改动，则需要重新插入
1. 核心原则：以“文件”为单位的生命周期管理
你需要转变一个观念：数据库里的数据不仅仅属于“某一天”，它首先属于**“某一个文件”**。

旧观念：DB 里有 2024-01-01 的数据。

新观念：DB 里有一堆数据，它们是由 raw_logs/2024-01.txt 这个文件产生的。

2. 数据库层面的改造 (Schema Change)
你需要让数据库记住数据的“来源”。

第一步：新建 source_files 表
这张表用来记录你处理过哪些文件，以及它们当时的状态。

SQL
CREATE TABLE source_files (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    file_path TEXT UNIQUE NOT NULL,  -- 例如 "raw_logs/2024-01.txt"
    file_hash TEXT NOT NULL,         -- SHA-256 哈希值
    last_imported_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
第二步：在 days 表（或主活动表）增加外键
给你的主数据表打上来源标签。

SQL
ALTER TABLE days ADD COLUMN source_file_id INTEGER REFERENCES source_files(id);
-- 建议加上索引，因为删除时会频繁用到
CREATE INDEX idx_days_source_file ON days(source_file_id);
3. 增量更新的执行流程 (The Workflow)
当用户执行 import 命令时，Core 不需要盲目解析所有文件，而是按照以下步骤操作：

阶段一：扫描与决策 (Scan & Decide)
遍历目录：扫描 raw_logs/ 下的所有 .txt 文件。

计算哈希：对每个文件（比如 2024-01.txt）计算 SHA-256。

查询对比：

去 source_files 表里查：这个路径存在吗？哈希一样吗？

情况 A (未变)：哈希一致 -> 跳过 (SKIP)。

情况 B (新增)：路径不存在 -> 标记为 待插入。

情况 C (修改)：路径存在但哈希变了 -> 标记为 待更新。

阶段二：处理变更 (Process Changes)
对于标记为“待更新”或“待插入”的文件（假设是 2024-01.txt），执行以下 原子事务：

解析 (Parse)：

读取 2024-01.txt。

生成 DailyLog 结构体（包含该月所有天的数据）。

开启事务 (Begin Transaction)：

这是防止数据跑偏的关键。

清理旧帐 (Clean Up - 关键步骤)：

如果数据库里已经有 2024-01.txt 的记录（即“情况 C”），执行：

SQL
DELETE FROM days WHERE source_file_id = (SELECT id FROM source_files WHERE file_path = '2024-01.txt');
注意：这里不要用 DELETE WHERE date BETWEEN '2024-01-01' AND '2024-01-31'。

原因：万一用户手滑，在 2024-01.txt 里写了一行 2023-12-31 的数据呢？如果你只删 1 月份的，那条错误的 12 月数据就会变成“僵尸”永远留在库里。按 source_file_id 删，能确保把这个文件上次带来的所有“垃圾”全带走。

插入新帐 (Insert New)：

获取或创建 source_files 里的记录 ID。

将解析出的数据插入 days 表，并把 source_file_id 填进去。

更新元数据 (Update Metadata)：

更新 source_files 表中该文件的 file_hash 为最新的哈希值。

提交事务 (Commit)。

4. 需要处理的特殊情况
情况一：文件被删除了
如果用户觉得某个月写得太烂，直接把 2024-01.txt 删了。

逻辑：你的扫描器会发现 DB 里有 2024-01.txt 的记录，但磁盘上没这个文件了。

操作：执行清理逻辑，DELETE FROM days WHERE source_file_id = ...，然后从 source_files 表里删掉该文件的记录。

情况二：文件改名了
用户把 2024-01.txt 改名为 Jan_2024.txt。

逻辑：系统会认为 2024-01.txt 被删除了（执行清理），同时 Jan_2024.txt 是个新文件（执行插入）。

结果：数据没丢，只是对应的 ID 变了。这是符合预期的。

5. 总结：你的任务清单
SQLite 修改：创建 source_files 表，修改 days 表。

Importer 修改：

不再只接收 vector<DailyLog>。

需要接收 filename 上下文。

实现 import_file(filename, data, hash) 方法，在里面包办“删旧+插新”的事务。

Core 调度修改：

在调用 Importer 前，先做一轮“哈希体检”。


1. 核心思路：数据库作为“胶水”
虽然文件是物理隔离的，但数据库里的数据是连续的。在处理一个新的月份文件之前，先去数据库里“偷看”一眼上个月最后发生了什么。

你需要修改 Pipeline 的调度逻辑，在调用 Converter 之前增加一步 “上下文获取”。

2. 具体实施步骤
步骤 A：Core 层的调度修改 (PipelineManager / ImportService)
在准备解析某个文件（如 2024-02.txt）之前：

计算前置时间点：根据文件名解析出该月的起始日期（2024-02-01）。

查询数据库上下文：

向 DB 发送查询：SELECT end_ts FROM activities WHERE end_ts < '2024-02-01' ORDER BY end_ts DESC LIMIT 1;

这会拿到 1 月 31 日最后那个活动的结束时间（比如 23:30）。

注入配置：将这个 23:30 作为一个 initial_last_end_time 参数，传递给 Converter。

步骤 B：Converter 层的逻辑增强 (LogLinker)
修改 LogLinker，让它支持“带状态启动”。

C++
// 伪代码示例
struct LogLinkerOptions {
    std::optional<TimePoint> initial_context_time; // 来自数据库的“上个月最后时刻”
};

class LogLinker {
public:
    std::vector<DailyLog> link_logs(std::vector<RawLog> raw_stream, LogLinkerOptions options) {
        Context context;
        
        // 关键点：如果外界注入了上下文，就用它初始化；否则用当天 00:00
        if (options.initial_context_time.has_value()) {
            context.last_end_time = options.initial_context_time.value();
        } else {
            // 只有在完全没有历史数据的“盘古开天地”时刻，才默认为当天起始
            context.last_end_time = derive_start_of_day(raw_stream.front()); 
        }

        // ... 继续执行你原本的循环逻辑 ...
        // 当处理第一条记录（如 08:00 起床）时：
        // Duration = 08:00 (Current) - 23:30 (Context from Jan)
        // 自动生成的 Sleep 就会非常准确。
    }
};
3. 处理“连锁反应” (The Ripple Effect)
这里还有一个隐蔽的 Bug：如果你修改了 1 月份的数据，2 月份的睡眠时长可能就错了。

场景：

数据库里有 1 月和 2 月的数据。

你修改了 2024-01.txt，把 1 月 31 日最后的睡觉时间从 23:30 改成了 22:00。

系统检测到哈希变化，重写了 1 月份的数据。

问题：此时 2 月份的数据（2024-02.txt）哈希没变，系统选择跳过。

结果：数据库里 2 月 1 日的睡眠时长依然是基于旧的 23:30 算的，数据不一致了。

解决方案：脏检查 (Dirty Check)
在 ImportService 完成“1 月份”的插入操作后，执行一个轻量级的 “边界一致性检查”：

获取当前文件（1月）的新结束时间：New_Jan_End = 22:00。

查询下一条记录的开始时间：查 DB 中大于 1 月的最早一条记录，即 2 月 1 日的那条数据的 start_ts。

对比：

如果 DB_Feb_Start (23:30) != New_Jan_End (22:00)：说明链条断了。

触发连锁更新：

系统自动把 2024-02.txt 也标记为“待更新”（即使哈希没变）。

强制重新解析 2024-02.txt（此时会用到新的 1 月上下文），从而修正 2 月的睡眠数据。

总结
为了不冲突：

向前看 (Look Backward)：解析 File N 时，从 DB 注入 File N-1 的结束时间。

向后踢 (Kick Forward)：更新 File N 后，检查是否打断了与 File N+1 的链接。如果断了，强制重跑 File N+1。



2
core要增加一个插入并且导出全部日期全部格式的方法，然后让cli调用

1
日报告的typ中 出现\正常吗
+ *Remark:* 没有背叛小到可以容忍 \
  没有背叛小到可以容忍 \
  minecraft真好玩 \
  皇室战争真好玩






1reports模块把数据获取和格式化解耦
让格式化不再设计数据库操作
shared 数据库代码，daily = 纯业务逻辑 + 报告生成


2导出的日报告的时候，按照年份文件夹，月份文件夹来分类

3
自定义标题的输出顺序
reports文件夹中json自定义格式化输出的标题，输出为true ,不输出为false

7
在core中提取时间统计输出格式，复用代码


6
检验dll是否缺失的环节，是否可以默认不打印，出问题才打印？
例如这些内容
正在初始化文件控制器...
主应用配置加载成功。
正在验证预处理配置文件...
[Validator] All preprocessing configuration data is valid.
正在验证查询配置文件...
[Validator] All query configuration files are valid.
正在验证插件...
[Validator] Validating required plugins in: C:\Base1\my_program\my_time_master\tm\plugins
[Validator] -- Found required plugin: DayMdFormatter
[Validator] -- Found required plugin: DayTexFormatter
[Validator] -- Found required plugin: DayTypFormatter
[Validator] -- Found required plugin: MonthMdFormatter
[Validator] -- Found required plugin: MonthTexFormatter
[Validator] -- Found required plugin: MonthTypFormatter
[Validator] -- Found required plugin: PeriodMdFormatter
[Validator] -- Found required plugin: PeriodTexFormatter
[Validator] -- Found required plugin: PeriodTypFormatter
[Validator] All required plugins were found.
[Validator] Validating required plugins in: C:\Base1\my_program\my_time_master\tm
[Validator] -- Found required plugin: reports_shared
[Validator] All required plugins were found.

7
converter里面json使用yyjson
inserter里面json插入使用yyjson
yyjson在pacman没有包，你需要手动放入程序目录


中优先级
toml配置重写修改命名和键名，以适配现在的程序

-3
python测试程序需要有一个一步一步插入的模式validate->convert->import

-2 新建创建一个独立的 analytics（或 query）模块

模块一：基础检索与回顾 (Basic Search & Review)
1. 全局备注搜索
目标：快速找到“那一天”或“那一件事”。
应用场景: "我哪天在备注里提到了 'minecraft'？" 或 "查找包含 '#我是备注1234' 的所有活动。"

2. 专注块过滤器
功能描述: 查找单次时长超过 N 小时的活动记录。

应用场景: "上个月我有多少次超过 3 小时的连续 '数据结构' 学习？"

3. 特定项目历史回顾 (Project History)
功能描述: 列出某个项目（如“高等数学”）在指定时间段内的所有记录。

应用场景: "我想看这周所有的英语文章阅读记录。"


模块二：习惯与趋势分析 (Habits & Trends)

4. 作息规律分析 (Sleep & Wake-up Trends)

功能描述: 绘制起床时间 (getup_time) 和入睡时间曲线。

应用场景: "最近是否越来越晚睡了？" "我有几天是 6:30 之前起床的？"

技术点: days.getup_time 是字符串，需要转化为分钟数以便绘图或计算平均值。

5. 熬夜/健康预警 (Health Alerts)

功能描述: 查找睡眠不足或作息异常的日子。

应用场景: "列出所有睡眠总时间 (sleep_total_time) 小于 6 小时的日子。" (你的需求之一)

6. 娱乐/学习比率 (Life Balance Ratio)

功能描述: 计算某段时间内“学习时长”与“娱乐时长”的比例。

应用场景: "这个月我的学习时间是否超过了刷抖音+B站的时间？"

模块三：连胜与打卡 (Streaks & Heatmaps)
目标：游戏化你的时间记录，提供正向反馈。

7. 连续打卡统计 (Streaks)

功能描述: 计算连续多少天进行了某项活动（如“英语文章”或“背单词”）。

应用场景: "我已经连续学了多少天英语了？"

技术点: 这是一个经典的 SQL "Gaps and Islands" 问题，需要在 C++ 层处理逻辑，查询所有包含该项目 ID 的日期并计算连续性。

8. 年度热力图数据 (GitHub-style Heatmap)

功能描述: 获取一年中每一天的特定指标强度。

应用场景: 生成类似 GitHub 贡献图的“学习热力图”或“运动热力图”。

模块四：关联洞察 (Correlation Insights)
目标：发现数据背后的隐性逻辑。

9. "玩物丧志"分析 (Correlation Analysis)

功能描述: 分析某项娱乐活动时长与当天学习时长的相关性。

应用场景: "是不是我白天打《守望先锋》的时间越长，晚上的学习效率越低？"

技术点: 查询 gaming_time 和 study_time 的数据对，计算相关系数（可以在 C++ 中计算）。

10. 关键词画像 (Tag Profiling)

功能描述: 基于备注标签统计时间。

应用场景: 你的文本中有 #我是备注1234 这种 Tag。可以统计带有特定 Tag 的任务总耗时。


技术点: 需要解析 activity_remark 字段中的 # 标签。


-1. 给txt文本记录要求写上规则(文档)




关注点: 逻辑判断、数据过滤、统计聚合、趋势发现。

输出: 数据实体对象 (vector<TimeRecord>, vector<DayEntry>) 或 统计数值。



-1
import的时候要检查是否为所需的 json文件，以免用户import了txt文件

7
使用字体的时候注意，可变字体必须是当前字体支持的语言
你用新罗马那肯定不能显示中文字体
用宋体也是不支持西文字体可变
用新罗马才支持西文字体可变

tex无法编译是因为不支持当前的可变字体，tex内容由加粗
解决方法就是代码中取消注释，手动指定字体的目录，但是这样只能编译，无法正确渲染字体加粗




低优先级




0. 核心逻辑库 (Core Library) 的接口设计

不要局限于“现在就编译成 DLL”，而是应该关注 “核心逻辑库 (Core Library) 的接口设计”。

核心就是这两件事：主要模块剥离逻辑和定义 C 接口。

代码解耦（现在做）： 将 Data Preprocessing、DB Insertion、Export 三个部分的代码从 CLI 的 main 逻辑中剥离出来，放入独立的库项目（Library Project）中。

定义 C-API 边界（关键）： 为这三个模块设计一层纯 C 的头文件接口。无论内部是用 C++ 还是未来用 Rust 实现，对外都只暴露这一层 C 接口。

示例： 不要传递 std::string，而是传递 const char* 和长度。

构建系统配置（CMake/Meson）： 配置你的构建脚本，使其支持通过开关切换 静态链接 (Static Lib) 和 动态链接 (Shared Lib/DLL)。

CLI 阶段： 建议优先使用静态链接。这样生成的 EXE 单文件便于分发，且编译器能进行全局优化 (LTO)。

移动端/Rust 替换阶段： 基于之前定义的 C-API，你可以随时用 Rust 编写实现，并编译成对应平台的动态库（.dll/.so）或静态库（.a），供上层调用。

4. 移动端复用路径
如果采用上述架构，未来在手机上复用的路径如下：

Android (JNI): Java/Kotlin 层通过 JNI 调用你的 C/C++ (或 Rust) 编译出的 .so 库。

iOS (FFI): Swift/Obj-C 直接调用编译出的静态库 .a 或 Framework。

Flutter/React Native: 通过 FFI (Foreign Function Interface) 调用 C 接口。








未来
1 
第一步（必须做）： 将所有 std::cout/cerr 替换为 ILogger 回调。这能解决“其他平台看不见日志”的问题。

第二步（优化）： 将“业务结果”（如错误列表、耗时统计）作为函数的返回值返回，而不是在函数内部处理（打印）。让调用者决定如何展示这些数据。

这会使你的 reprocessing 模块变成一个纯粹的库（Library），可以被任何程序调用。


为了实现彻底的模块化并支持多平台（如手机 App、GUI、后台服务），核心业务逻辑模块（如 reprocessing 和 PipelineManager）绝对不能包含 std::cout、std::cerr 或 printf。

当前代码中的 std::cout 是硬编码的控制台输出，这在手机 App（没有控制台）或 GUI（需要弹窗或进度条）中是完全不可见的，甚至会导致崩溃或资源浪费。

如何解耦？（架构改造方案）
你需要将“发生什么（Logic）”与“如何通知用户（Presentation）”分离开。通常有两种互补的方法来实现这一点：

方法 1：引入抽象日志接口 (Dependency Injection)
这是最标准的方法。让 reprocessing 模块不依赖具体的 iostream，而是依赖一个抽象的接口。

2. 和sliqte解耦，采取注册模式?支持多种数据库



















## 让tex支持生成的字体从本地获取
\setCJKmainfont[Path=C:/]{NotoSerifSC-VariableFont_wght.ttf}

还没有给json验证模块验证total_exercise_time










## tex格式增加更细致的字体控制
typ全格式已经实现


## clang开启连接时优化需要下载对应工具
pacman -S mingw-w64-ucrt-x86_64-lld


## 格式化输出，根据类型输出不同的颜色背景。
没有任何一个实现


## json中的存入数据库但是还没使用的键

                "endTimestamp": 1735699680,
                "startTimestamp": 1735693800

            "anaerobicTime": 8220,// 已经在日报告中实现
            "cardioTime": 0,// 已经在日报告中实现
            "gamingTime": 12540,
            "groomingTime": 0,// 已经在日报告中实现
            "sleepDayTime": 8460,
            "sleepNightTime": 19380,
            "sleepTotalTime": 27840, // 已经在日报告中实现
            "toiletTime": 0,
            "totalExerciseTime": 16140


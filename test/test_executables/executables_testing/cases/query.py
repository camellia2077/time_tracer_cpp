# test/cases/query.py
from ..core.base import BaseTester, TestCounter
from ..conf.definitions import TestContext, TestReport, Colors

class QueryTester(BaseTester):
    def __init__(self, counter: TestCounter, module_order: int, context: TestContext,
                 generated_db_file_name: str, day_dates: list, month_dates: list,
                 week_params: list, year_params: list,
                 recent_days: list, range_dates: list, test_formats: list):
        super().__init__(counter, module_order, "query", context)
        self.db_file = context.db_path if context.db_path else context.exe_path.parent / generated_db_file_name
        self.day_dates = day_dates
        self.month_dates = month_dates
        self.week_params = week_params
        self.year_params = year_params
        self.recent_days = recent_days
        self.range_dates = range_dates
        self.formats = test_formats

    def run_tests(self) -> TestReport:
        report = TestReport(module_name=self.module_name)
        
        if not self.db_file.exists():
            print(f"{Colors.RED}错误: 数据库文件不存在: {self.db_file}{Colors.RESET}")
            return report

        db_arg = ["--database", str(self.db_file)]
        
        tests_to_run = []
        for date in self.day_dates:
            for fmt in self.formats:
                tests_to_run.append((f"Query Day({date}) [{fmt}]", ["query", "day", date, "--format", fmt] + db_arg))
        for month in self.month_dates:
            for fmt in self.formats:
                tests_to_run.append((f"Query Month ({month}) [{fmt}]", ["query", "month", month, "--format", fmt] + db_arg))
        for year, week in self.week_params:
            for fmt in self.formats:
                tests_to_run.append((f"Query Week ({year} W{week}) [{fmt}]", ["query", "week", str(year), str(week), "--format", fmt] + db_arg))
        for year in self.year_params:
            for fmt in self.formats:
                tests_to_run.append((f"Query Year ({year}) [{fmt}]", ["query", "year", str(year), "--format", fmt] + db_arg))
        for days in self.recent_days:
            for fmt in self.formats:
                tests_to_run.append((f"Query Recent ({days}) [{fmt}]", ["query", "recent", str(days), "--format", fmt] + db_arg))
        for start, end in self.range_dates:
            for fmt in self.formats:
                tests_to_run.append((f"Query Range ({start} to {end}) [{fmt}]", ["query", "range", start, end, "--format", fmt] + db_arg))

        for name, args in tests_to_run:
            res = self.run_command_test(name, args)
            report.results.append(res)
        
        return report
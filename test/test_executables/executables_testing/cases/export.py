# test/cases/export.py
from pathlib import Path
from typing import List, Tuple
from ..core.base import BaseTester, TestCounter
from ..conf.definitions import Colors, TestContext, TestReport, SingleTestResult

class ExportTester(BaseTester):
    def __init__(self, counter: TestCounter, module_order: int, context: TestContext,
                 generated_db_file_name: str, 
                 export_mode: str,
                 specific_dates: list,
                 specific_months: list,
                 specific_weeks: list,
                 specific_years: list,
                 specific_recent: list,
                 specific_range: list,
                 test_formats: list,
                 export_output_path: Path):
                 
        super().__init__(counter, module_order, "export", context)
        
        self.db_file = context.db_path if context.db_path else context.exe_path.parent / generated_db_file_name
        self.export_output_path = export_output_path
        self.export_mode = export_mode
        self.specific_dates = specific_dates
        self.specific_months = specific_months
        self.specific_weeks = specific_weeks
        self.specific_years = specific_years
        self.specific_recent_days = specific_recent
        self.specific_range_dates = specific_range
        self.formats = test_formats

    def run_tests(self) -> TestReport:
        """主入口：逻辑清晰，只负责流程控制"""
        report = TestReport(module_name=self.module_name)
        
        # 1. 前置检查
        if not self._check_db_exists(report):
            return report

        # 2. 规划测试用例 (Planning)
        # 获取所有要跑的测试 (name, args) 列表
        test_cases = self._plan_test_cases()

        # 3. 执行测试用例 (Execution)
        for name, args in test_cases:
            # add_output_dir=False 因为我们在 common_args 里已经指定了
            res = self.run_command_test(name, args, add_output_dir=False)
            report.results.append(res)
                
        return report

    # --- 辅助方法 (Helpers) ---

    def _check_db_exists(self, report: TestReport) -> bool:
        """检查数据库是否存在，若不存在则填充失败报告"""
        if not self.db_file.exists():
            fail_res = SingleTestResult(
                name="Database Existence Check",
                status="FAIL",
                messages=[f"{Colors.RED}错误: 数据库文件不存在: {self.db_file}{Colors.RESET}"]
            )
            report.results.append(fail_res)
            return False
        return True

    def _plan_test_cases(self) -> List[Tuple[str, List[str]]]:
        """根据配置生成测试计划"""
        common_args = [
            "--database", str(self.db_file),
            "--output", str(self.export_output_path)
        ]

        if self.export_mode == "bulk":
            return self._build_bulk_cases(common_args)
        elif self.export_mode == "specific":
            return self._build_specific_cases(common_args)
        else:
            return []

    def _build_bulk_cases(self, common_args: List[str]) -> List[Tuple[str, List[str]]]:
        """构建批量导出模式的测试用例"""
        cases = []
        
        # 1. day & month & week & year
        cases.extend(self._make_cases("Bulk Export All Day", ["export", "all-day"], common_args))
        cases.extend(self._make_cases("Bulk Export All Month", ["export", "all-month"], common_args))
        cases.extend(self._make_cases("Bulk Export All Week", ["export", "all-week"], common_args))
        cases.extend(self._make_cases("Bulk Export All Year", ["export", "all-year"], common_args))

        # 2. recent
        if self.specific_recent_days:
            recent_days_str = ",".join(map(str, self.specific_recent_days))
            cases.extend(self._make_cases(
                "Bulk Export All Recent", 
                ["export", "all-recent", recent_days_str], 
                common_args
            ))
        return cases

    def _build_specific_cases(self, common_args: List[str]) -> List[Tuple[str, List[str]]]:
        """构建指定导出模式的测试用例"""
        cases = []

        # 1. Specific Day
        for date in self.specific_dates:
            cases.extend(self._make_cases(
                f"Specific Export Day ({date})", 
                ["export", "day", date], 
                common_args
            ))

        # 2. Specific Month
        for month in self.specific_months:
            cases.extend(self._make_cases(
                f"Specific Export Month ({month})", 
                ["export", "month", month], 
                common_args
            ))
            
        # 3. Specific Week
        for year, week in self.specific_weeks:
            cases.extend(self._make_cases(
                f"Specific Export Week ({year} W{week})", 
                ["export", "week", str(year), str(week)], 
                common_args
            ))

        # 4. Specific Year
        for year in self.specific_years:
            cases.extend(self._make_cases(
                f"Specific Export Year ({year})", 
                ["export", "year", str(year)], 
                common_args
            ))

        # 5. Specific Recent
        for days in self.specific_recent_days:
            cases.extend(self._make_cases(
                f"Specific Recent Export ({days} days)", 
                ["export", "recent", str(days)], 
                common_args
            ))

        # 6. Specific Range
        for start, end in self.specific_range_dates:
            cases.extend(self._make_cases(
                f"Specific Range Export ({start} to {end})", 
                ["export", "range", start, end], 
                common_args
            ))
            
        return cases

    def _make_cases(self, base_name: str, base_cmd: List[str], common_args: List[str]) -> List[Tuple[str, List[str]]]:
        """
        微型工厂：为每个 format 生成一个测试用例
        """
        cases = []
        for fmt in self.formats:
            full_name = f"{base_name} [{fmt.upper()}]"
            full_cmd = base_cmd + ["--format", fmt] + common_args
            cases.append((full_name, full_cmd))
        return cases
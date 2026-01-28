# Time Tracer CLI

This directory contains the CLI implementation for Time Tracer.

## Command Naming Convention

All date-related commands and sub-commands MUST use **nouns** instead of adverbs (no "-ly" suffixes).

### Querying Reports
- `query day <date>` (e.g., `query day 2026-01-28`)
- `query week <year> <week_num>`
- `query month <year-month>`
- `query year <year>`
- `query recent <days>`
- `query range <start_date> <end_date>`

### Exporting Reports
- `export day <date>`
- `export week <year> <week_num>`
- `export month <year-month>`
- `export year <year>`
- `export recent <days>`
- `export range <start_date> <end_date>`

### Batch Exporting
- `export all-day`
- `export all-week`
- `export all-month`
- `export all-year`
- `export all-recent <days_list>`

## Development Guidelines
When adding new report types, ensure the sub-command name is a simple noun. Do not use `daily`, `weekly`, `monthly`, or `yearly`.

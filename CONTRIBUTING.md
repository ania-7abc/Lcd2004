# Contributing to Lcd2004

Thanks for wanting to contribute! 

## Getting started
1. Fork the repository
2. Clone your fork: `git clone https://github.com/your-username/Lcd2004.git`
3. Create a branch: `git checkout -b feature/your-feature`

## Development
- Keep changes consistent with existing code style
- All headers are in `include/`. Examples live in `examples/` organized by use case
- Test your changes on hardware if possible, or at least ensure examples compile
- If you add public API, update README and/or docs in `docs/`
- For compile-time behavior, prefer macros consistent with existing ones

## Testing
For quick compile tests locally:
```bash
pio run -e uno
```

Or use Arduino CLI / PlatformIO to compile examples in `examples/`.

## Pull requests
- Keep PRs focused on a single change
- Describe what you changed and why
- Fill out the PR template
- Link related issues if any

## Reporting bugs
Use the GitHub Issue form. Include library version, board/MCU, connection type, display size, class used, minimal sketch, and expected vs actual behavior.

## License
By contributing, you agree that your contributions will be licensed under the MIT License.

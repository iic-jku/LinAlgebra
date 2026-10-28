# Project conventions

## Repositories

- This is `iic-jku/LinAlgebra` (remote `jku`, formerly `martinjankoehler/LinAlgebra`), a fork
  of `ediloren/LinAlgebra` (remote `upstream`). Remotes use SSH URLs (`git@github.com:…`).
- `iic-jku/FasterCap` builds it as the sibling `../LinAlgebra` (`add_subdirectory`); its CI
  checks out this fork's `master`. Locally, `LinAlgebra` (branch `merged`) is what
  `FasterCap_MJK` builds, and `LinAlgebra_MJK` holds the topic branches.
- Fixes meant for upstream go on branches based on `upstream/master`, one concern each, as
  for the open PRs `ediloren/LinAlgebra#1`–`#4`.
- `LinAlgebraTest` (`martinjankoehler/LinAlgebraTest`) compares timings and results of the
  original and the fork.

## Scope

- One concern per branch; report unrelated findings instead of fixing them.
- Say in the commit body when behavior changes: results, signatures, build options.
- Fix root causes in the shared code, not with special cases.

## Code

- Keep each file's line endings (CRLF), indentation (tabs) and naming (`CLin_…`, `m_i…`).
  `STDAFX.H` stays the first include of every `.cpp`.
- `CLin_Matrix` stores rows contiguously (`m_pV`, row pointers `m_pRow`): BLAS calls use
  `CblasRowMajor`, and the leading dimension is the number of columns of the stored
  matrix. `CLin_subscript` is `unsigned long`; BLAS integers are `int` (OpenBLAS LP64) or
  `long` (Accelerate ILP64).
- BLAS functions abort or return garbage on invalid arguments: keep the size checks of the
  original loops (`CLin_assert`, `LINALG_BOUNDS_CHECK`), including empty vectors and
  matrices.
- CMake: find BLAS once here and export it with the `LinAlgebra` target
  (`target_link_libraries(LinAlgebra PUBLIC BLAS::BLAS)`), so FasterCap doesn't repeat the
  detection. On macOS the Accelerate headers and library must come together.
- Don't duplicate code: extend the shared code rather than adding a variant.

## Tests

- There is no test suite here. Each bug fix gets a test that fails without it, comparing the
  BLAS path with the original loops on non-square, square, `1×N` and empty shapes.
- Check FasterCap after every change: build `FasterCap_MJK` against it and compare a
  reference run with the build before (same machine, same build type).
- No loosened tolerances without a numerical explanation (summation order of BLAS kernels
  is one; differences beyond a few ulps per element are not).

## Git

- Martin reviews in detail and commits himself: leave changes uncommitted in the working
  copy. For a change that should become several commits, stage it and propose the split.
  Proposed commit messages are welcome as a starting point. Don't commit or push unless
  asked; never commit scratch files.
- Martin keeps WIP stashes: never chain `git stash push`/`pop`. Test staged changes via
  `git checkout-index -a --prefix=<dir>/` or a temporary worktree.
- Commit subject: `Area: verb phrase`, 6–12 words, no articles. Area is a file, class or
  function, e.g. `Mtx: rewrote matrix multiplication with faster BLAS-based impl`,
  `CMakeLists.txt: fixed project name`. Usually no body; one or two lines for a non-obvious
  why or a behavior change.
- Commit and PR texts contain only what the reader can't get from the issue, diff or CI.
  PR: issue reference plus 1–4 sentences, no Summary/Changes/Testing headings, and a
  proposed title in the style of the commit subjects.
- Every PR for a bug references an issue: `Fixes #N` or `Part of #N`; without an issue,
  file one first. Issues in other repos are written `ediloren/LinAlgebra#N`,
  `iic-jku/FasterCap#N`.
- Issues show the smallest reproducing input and end with the versions used: commit, OS and
  CPU architecture, compiler, BLAS library and its integer model.
- Prefer bullet lists to long sentences chained with semicolons; no period at the end of a
  bullet. Mark identifiers as code with backticks, every mention.
- No AI attribution: no `Co-Authored-By` trailer, no "Generated with …" line.
- Before handing over, re-read your diff against `master` and check it against this file.

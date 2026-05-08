# demo-c-random-crash

A tiny C program that crashes intermittently. Designed as a 2-min demo
for [neverbreak.ai](https://neverbreak.ai).

## What it does

The program runs 12 rounds. In each round it picks a number and processes it:

- Rounds 1-4 are warmup (crash disabled).
- From round 5 onward, certain numbers cause the program to mark itself as
  "bad" and crash on the **next** round with a `SIGSEGV`.
- Round 7 forces the trigger condition, so CI is guaranteed to fail at
  round 8.

Total runtime to crash: ~80 seconds.

This is the kind of intermittent CI failure that's painful to debug in
practice — the failure mode depends on the random sequence, so reproducing
it locally usually means re-running the test until it happens to crash
again. neverbreak.ai turns it into a one-shot, repeatable failure with
the full debug context attached.

## How to use it with neverbreak.ai

1. **Fork this repo** to your account.
2. **Install the [neverbreak GitHub App](https://github.com/apps/neverbreak)**
   on your fork.
3. **Push any commit** to your fork. CI will fail at round 8, and neverbreak
   will:
   - Reproduce the failure on demand
   - Capture everything you need to debug it
   - Suggest a fix, verify it, and open a PR

You'll see the bug appear on your dashboard at
[app.neverbreak.ai](https://app.neverbreak.ai) within ~2 minutes.

## Build & run locally

```sh
make build   # compile
make test    # build + run; will crash at round 8
make clean   # remove binary
```

## Files

| File                                | Purpose                              |
|-------------------------------------|--------------------------------------|
| `demo_crash.c`                      | The demo program                     |
| `Makefile`                          | `build`, `test`, `clean` targets     |
| `.github/workflows/neverbreak.yml`  | CI workflow (runs `make test`)       |

## License

Public domain. Fork it, modify it, break it however you like.

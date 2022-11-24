### Only exec
Only generates the output
```
make run                // execs sequential
make run NUM_THREADS=N  // execs using N threads
```

### Exec using gprof 
Generates the output and the flat profile using gprof (saves as profile)
```
make gprof                // execs sequential
make gprof NUM_THREADS=N  // execs using N threads
```

### Exec using clock 
Generates the output and measure the time using clock (saves as profile)
```
make profile                // execs sequential
make profile NUM_THREADS=N  // execs using N threads
```

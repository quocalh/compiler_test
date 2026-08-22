echo "run"

# cleaning up the old prog
if [ -f ./.tmp/prog ]; then
  rm -rf ./.tmp/prog
fi

echo "running the main.c"
# gcc main.c ./modules/*.c ./misc/*.c -I . -I ./misc -o .tmp/prog || echo "what the fuck" && return 1 2>/dev/null

if ! gcc test.c ./modules/*.c ./misc/*.c -I . -I ./misc -o .tmp/prog; then
  echo "what the fuck"
  return 1
fi

echo "compiled successfully"
./.tmp/prog

set -e
for filename in `ls src/*.bs.js`
do
diff $filename ~/backup-bs/$filename
done
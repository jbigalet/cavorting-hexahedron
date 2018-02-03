import glob, re, sys
R = re.compile('#include "([^"]*)"')

all_deps = {}
files = sys.argv[1:]
#sys.stderr.write(" ".join(files))
q = files[:]
while len(q) > 0:
	fn = q.pop()
	f = open(fn, "r")
	content = f.read()
	all_deps[fn] = []
	for dep in R.findall(content):
		all_deps[fn].append(dep)
		if dep in all_deps: continue
		if dep in files: continue
		q.append(dep)
	f.close()
f = open("dependencies.make", "w")
for fn in files:
	done = {}
	q = [fn]
	f.write("build/" + fn[:-4] + ".o: ")
	while len(q) > 0:
		d = q.pop()
		done[d] = True
		f.write(d + " ")
		if not d in all_deps: continue
		for dep in all_deps[d]:
			if dep in done: continue
			if dep in q: continue
			q.append(dep)
	f.write("\n")
f.close()

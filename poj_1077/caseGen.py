import numpy as np

nums = range(1,10)
nums[8] = 'x'
perm = []
for ii in range(9):
    id = np.random.randint(len(nums))
    perm.append(nums[id])
    nums.remove(nums[id])
print(' '.join(map(str,perm)))

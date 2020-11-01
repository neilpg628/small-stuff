function [bfs, opt, cost, dir] = getbfs(A, b, o)

	[row, col] = size(A);
	
	C = nchoosek(1:col,row);
	c = nchoosek(col,row);
	
	solutions = zeros(c,col);

	for i = 1:c
		perm = C(i,:);
		vec = A(:,perm)\b(:);
		if sum(isinf(vec)) == 0
			res = zeros(col,1);
			res(perm) = vec;
			solutions(i,:) = res';
		end
	end

	solu = unique(solutions, 'rows');

	ind = sum((solu  == abs(solu) & sum(solu,2) > 0), 2) == col;

	bfs = solu(ind,:);
	
	[num, len] = size(bfs);
	cost = zeros(num, 1);
	obj = zeros(len, 1);
	obj(1:length(o)) = o;
	
	for i = 1:num
		cost(i) = dot(bfs(i,:), obj);
	end
	
	[M, I] = max(cost);
	
	opt = bfs(I, :);
	
	
	[~, indices] = ismember(bfs, solutions, 'rows');
	basic = C(indices, :);
	dir = zeros(len, len, num);
	
	for i = 1:num
		sele = basic(i, :);
		for j = 1:len
			direc = zeros(1, len);
			direc(j) = 1;
			db = zeros(1, len);
			db(sele) = -inv(A(:, sele))*A(:,j);
			direc = direc + db;
			dir(j, :, i) = direc;
		end
	end
	
	dir = squeeze(dir);
	
end


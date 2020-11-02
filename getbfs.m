function [bfs, opt, cost, directions] = getbfs(A, b, o)

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
	
	cost = bfs*obj(:);
	
	[~, I] = max(cost);
	
	opt = bfs(I, :);
	
	
	[~, indices] = ismember(bfs, solutions, 'rows');
	basic = C(indices, :);
	directions = zeros(len, len, num);
	
	for i = 1:num
		sele = basic(i, :);
		bcol = -inv(A(:, sele))*A;
		for j = 1:len
			direc = zeros(1, len);
			direc(j) = 1;
			db = zeros(1, len);
			db(sele) = bcol(:, j);
			direc = direc + db;
			directions(j, :, i) = direc;
		end
	end

end


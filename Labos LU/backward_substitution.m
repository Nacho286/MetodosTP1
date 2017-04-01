function y = backward_substitution(A,b)
   if size(b,2)~=1 || size(A,1)~=numel(b),
      error('las dimensiones no concuerdan') ;
   end ;
   
   if any( tril(A,-1) ),
      error('A no es triangular superior') ;
   end ;
   
   n = size(A,2) ;
   y = zeros(n,1) ;
   
   for i=n:-1:1,
      y(i) = b(i);
      for j = n:-1:i+1
          y(i) = y(i) - A(i,j)*y(j);
      end
      y(i) = y(i)/A(i,i);
   end
end
